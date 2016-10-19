#include "instro/clang/core/ClangConstruct.h"

#include "instro/utility/exception.h"
#define INSTRO_LOG_LEVEL INFO
#include "instro/utility/Logger.h"

#include "llvm/Support/raw_ostream.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/DeclVisitor.h"
#include "clang/AST/StmtVisitor.h"

#include "boost/algorithm/string/replace.hpp"
#include "boost/algorithm/string/trim.hpp"

using namespace InstRO::Core;
using namespace InstRO::Clang::Core;

namespace {

class PredicateMatcherHandler : public clang::ast_matchers::MatchFinder::MatchCallback {
 public:
	PredicateMatcherHandler() : matchedOnce(false) {}

	void run(const clang::ast_matchers::MatchFinder::MatchResult &result) override { matchedOnce = true; }

	bool matched() const { return matchedOnce; }

 private:
	bool matchedOnce;
};

class DeclConstructTraitVisitor : public clang::DeclVisitor<DeclConstructTraitVisitor> {
 public:
	DeclConstructTraitVisitor(clang::ASTContext &context) : context(context), ct(ConstructTraitType::CTNoTraits) {}
	ConstructTrait getConstructTrait() { return ct; }

	void VisitFunctionDecl(clang::FunctionDecl *decl) {
		if (decl->isThisDeclarationADefinition()) {
			ct = ConstructTrait(ConstructTraitType::CTFunction);
		} else {
			generateError(decl);
		}
	}

	void VisitTranslationUnitDecl(clang::TranslationUnitDecl *decl) {
		ct = ConstructTrait(ConstructTraitType::CTFileScope);
	}

	void VisitVarDecl(clang::VarDecl *decl) {
		if (decl->hasInit()) {
			// As of today we do not consider ctors as initializers (which is probably dubious)
			if (llvm::isa<clang::CXXConstructExpr>(decl->getInit())) {
				InstRO::logIt(InstRO::INFO) << "Skipping Constructor initializer" << std::endl;
				generateError(decl);
				return;
			}
			auto parents = context.getParents(*decl);
			for (auto parent : parents) {
				if (parent.get<clang::DeclStmt>()) {
					ct = ConstructTrait(ConstructTraitType::CTSimpleStatement);
					handleStatementWithWrappableCheck(decl);
					return;
				}
			}
		}
		generateError(decl);
	}

	void VisitDecl(clang::Decl *decl) { generateError(decl); }

 private:
	clang::ASTContext &context;
	ConstructTrait ct;

	void handleStatementWithWrappableCheck(clang::Decl *decl) {
		ct.add(ConstructTraitType::CTStatement);
		auto matcher = clang::ast_matchers::decl(clang::ast_matchers::hasParent(clang::ast_matchers::compoundStmt()));
		auto matches = clang::ast_matchers::match(matcher, *decl, context);
		if (!matches.empty()) {
			InstRO::logIt(InstRO::DEBUG) << "Matched a wrappable statement" << std::endl;
			ct.add(ConstructTraitType::CTWrappableStatement);
		}
	}

	void generateError(clang::Decl *decl) {
		InstRO::logIt(InstRO::INFO) << "Skipping declaration " << decl->getDeclKindName() << "(" << decl << ")"
																<< std::endl;
	}
};

class StmtConstructTraitVisitor : public clang::StmtVisitor<StmtConstructTraitVisitor> {
 public:
	StmtConstructTraitVisitor(clang::ASTContext &context) : context(context), ct(ConstructTraitType::CTNoTraits) {}
	ConstructTrait getContructTrait() { return ct; }

	void VisitBreakStmt(clang::BreakStmt *stmt) {
		ct = ConstructTrait(ConstructTraitType::CTSimpleStatement);
		ct.add(ConstructTraitType::CTStatement);
	}

	void VisitContinueStmt(clang::ContinueStmt *stmt) {
		ct = ConstructTrait(ConstructTraitType::CTSimpleStatement);
		ct.add(ConstructTraitType::CTStatement);
	}

	void VisitIfStmt(clang::IfStmt *stmt) {
		ct = ConstructTrait(ConstructTraitType::CTConditionalStatement);
		handleStatementWithWrappableCheck(stmt);
	}

	void VisitSwitchStmt(clang::SwitchStmt *stmt) {
		ct = ConstructTrait(ConstructTraitType::CTConditionalStatement);
		handleStatementWithWrappableCheck(stmt);
	}

	void VisitForStmt(clang::ForStmt *stmt) {
		ct = ConstructTrait(ConstructTraitType::CTLoopStatement);
		handleStatementWithWrappableCheck(stmt);
	}

	void VisitWhileStmt(clang::WhileStmt *stmt) {
		ct = ConstructTrait(ConstructTraitType::CTLoopStatement);
		handleStatementWithWrappableCheck(stmt);
	}

	void VisitDoStmt(clang::DoStmt *stmt) {
		ct = ConstructTrait(ConstructTraitType::CTLoopStatement);
		handleStatementWithWrappableCheck(stmt);
	}

	void VisitCompoundStmt(clang::CompoundStmt *stmt) {
		// ignore the top level compound statement of a function and switch statement
		auto parents = context.getParents(*stmt);
		if (parents.size() == 1) {
			if (parents[0].get<clang::FunctionDecl>() || parents[0].get<clang::SwitchStmt>()) {
				generateError(stmt);
			} else {
				ct = ConstructTrait(ConstructTraitType::CTScopeStatement);
				handleStatementWithWrappableCheck(stmt);
			}
		} else {
			InstRO::logIt(InstRO::DEBUG) << "Encountered a compound statement with more than one parent" << std::endl;
			generateError(stmt);
		}
	}

	void VisitDeclStmt(clang::DeclStmt *stmt) { generateError(stmt); }

	void VisitExpr(clang::Expr *stmt) {
		// in Clang every Expr is also a Stmt, therefore Expressions might also be SimpleStatements

		if (handleConstructsInControlStructures(stmt)) {
			return;
		}

		// We do not consider constructor expressions to be expressions
		if (llvm::isa<clang::CXXConstructExpr>(stmt)) {
			InstRO::logIt(InstRO::INFO) << "Skipping CXXConstructExpr in expression generation" << std::endl;
			return;
		}

		if (llvm::isa<clang::UnaryOperator>(stmt) || llvm::isa<clang::BinaryOperator>(stmt)) {
			// interesting unary or binary expressions have at least one variable reference or function call
			clang::ast_matchers::MatchFinder finder;
			PredicateMatcherHandler handler;
			finder.addMatcher(
					clang::ast_matchers::expr(clang::ast_matchers::hasDescendant(clang::ast_matchers::declRefExpr())), &handler);
			finder.addMatcher(clang::ast_matchers::expr(clang::ast_matchers::hasDescendant(clang::ast_matchers::callExpr())),
												&handler);
			finder.match(*stmt, context);
			if (!handler.matched()) {
				InstRO::logIt(InstRO::DEBUG) << "Encountered a boring operator expression" << std::endl;
				generateError(stmt);
				return;
			}

			if (clang::UnaryOperator *unaryOp = llvm::dyn_cast<clang::UnaryOperator>(stmt)) {
				if (unaryOp->getOpcode() == clang::UO_AddrOf) {
					InstRO::logIt(InstRO::DEBUG) << "Encountered a boring address-of operator" << std::endl;
					generateError(stmt);
					return;
				}
			}
		} else if (clang::CastExpr *castExpr = llvm::dyn_cast<clang::CastExpr>(stmt)) {
			if (castExpr->getCastKind() != clang::CastKind::CK_UserDefinedConversion) {
				InstRO::logIt(InstRO::DEBUG) << "Encountered a boring cast expression" << std::endl;
				generateError(stmt);
				return;
			}
		}

		bool isNotAStatement = false;
		auto parents = context.getParents(*stmt);
		if (parents.size() == 1) {
			if (const clang::Stmt *parent = parents[0].get<clang::Stmt>()) {
				isNotAStatement = llvm::isa<clang::Expr>(*parent);
				if (!isNotAStatement) {
					if (llvm::isa<clang::ReturnStmt>(parent)) {
						// the expression inside a return statement cannot be a statement
						isNotAStatement = true;
					}
				}
			} else if (parents[0].get<clang::Decl>()) {
				isNotAStatement = true;
			}
		}

		if (clang::BinaryOperator *binaryOp = llvm::dyn_cast<clang::BinaryOperator>(stmt)) {
			if (binaryOp->getOpcode() == clang::BO_Comma) {
				if (isNotAStatement) {
					InstRO::logIt(InstRO::DEBUG) << "Encountered a boring comma operator" << std::endl;
					generateError(stmt);
					return;
				} else {
					// use the top level comma operator as pure statement
					ct = ConstructTrait(ConstructTraitType::CTSimpleStatement);
					handleStatementWithWrappableCheck(stmt);
					return;
				}
			}
		}

		std::string nodeTypeName = stmt->getStmtClassName();
		ct = ConstructTrait(ConstructTraitType::CTExpression);
		if (!isNotAStatement) {
			ct.add(ConstructTraitType::CTSimpleStatement);
			handleStatementWithWrappableCheck(stmt);
		}
	}

	void VisitCXXThisExpr(clang::CXXThisExpr *stmt) {
		// ignore 'this'
		generateError(stmt);
	}

	void VisitParenExpr(clang::ParenExpr *stmt) {
		// ignore parentheses
		generateError(stmt);
	}

	void VisitCXXBoolLiteralExpr(clang::CXXBoolLiteralExpr *stmt) {
		if (!handleConstructsInControlStructures(stmt)) {
			// literals are not considered interesting
			generateError(stmt);
		}
	}

	void VisitIntegerLiteral(clang::IntegerLiteral *stmt) {
		if (!handleConstructsInControlStructures(stmt)) {
			// literals are not considered interesting
			generateError(stmt);
		}
	}

	void VisitFloatingLiteral(clang::FloatingLiteral *stmt) {
		if (!handleConstructsInControlStructures(stmt)) {
			// literals are not considered interesting
			generateError(stmt);
		}
	}

	void VisitCharacterLiteral(clang::CharacterLiteral *stmt) {
		if (!handleConstructsInControlStructures(stmt)) {
			// literals are not considered interesting
			generateError(stmt);
		}
	}

	void VisitStringLiteral(clang::StringLiteral *stmt) {
		if (!handleConstructsInControlStructures(stmt)) {
			// literals are not considered interesting
			generateError(stmt);
		}
	}

	void VisitCXXNullPtrLiteralExpr(clang::CXXNullPtrLiteralExpr *stmt) {
		if (!handleConstructsInControlStructures(stmt)) {
			// literals are not considered interesting
			generateError(stmt);
		}
	}

	void VisitDeclRefExpr(clang::DeclRefExpr *stmt) {
		if (!handleConstructsInControlStructures(stmt)) {
			// references to declarations (variables) are not considered interesting
			generateError(stmt);
		}
	}

	void VisitImplicitCastExpr(clang::ImplicitCastExpr *stmt) {
		// not considered interesting
		generateError(stmt);
	}

	void VisitUnresolvedLookupExpr(clang::UnresolvedLookupExpr *stmt) { generateError(stmt); }

	void VisitArraySubscriptExpr(clang::ArraySubscriptExpr *stmt) {
		// array subscript operator has no observable behavior
		// overloading the operator in a class should instead lead to a 'CXXOperatorCallExpr'
		generateError(stmt);
	}

	void VisitMemberExpr(clang::MemberExpr *stmt) {
		// used e.g. by CXXMemberCallExpr to specify the member being called
		generateError(stmt);
	}

	void VisitReturnStmt(clang::ReturnStmt *stmt) {
		ct = ConstructTrait(ConstructTraitType::CTSimpleStatement);
		ct.add(ConstructTraitType::CTStatement);
		// return statements are not wrappable
	}

	void VisitStmt(clang::Stmt *stmt) { generateError(stmt); }

	bool isExpressionInLoopOrConditionalHeader(const clang::Expr *expr) {
		auto parents = context.getParents(*expr);
		if (parents.empty()) {
			return false;
		}

		auto &parent = parents[0];
		if (const clang::IfStmt *ifStmt = parent.get<clang::IfStmt>()) {
			return ifStmt->getCond() == expr;
		} else if (const clang::SwitchStmt *switchStmt = parent.get<clang::SwitchStmt>()) {
			return switchStmt->getCond() == expr;
		} else if (const clang::ForStmt *forStmt = parent.get<clang::ForStmt>()) {
			return forStmt->getInc() == expr;
		} else if (const clang::DoStmt *doStmt = parent.get<clang::DoStmt>()) {
			return doStmt->getCond() == expr;
		} else if (const clang::WhileStmt *whileStmt = parent.get<clang::WhileStmt>()) {
			return whileStmt->getCond();
		} else if (const clang::ImplicitCastExpr *implCastExpr = parent.get<clang::ImplicitCastExpr>()) {
			return isExpressionInLoopOrConditionalHeader(implCastExpr);
		}

		return false;
	}

	bool isStatementInForLoop(clang::Stmt *stmt) {
		auto parents = context.getParents(*stmt);
		if (parents.empty()) {
			return false;
		}

		if (const clang::ForStmt *forStmt = parents[0].get<clang::ForStmt>()) {
			return forStmt->getCond() == stmt || forStmt->getInit() == stmt;
		}

		return false;
	}

 private:
	clang::ASTContext &context;
	ConstructTrait ct;

	bool handleConstructsInControlStructures(clang::Expr *stmt) {
		if (isExpressionInLoopOrConditionalHeader(stmt)) {
			ct = ConstructTrait(ConstructTraitType::CTExpression);
			return true;
		} else if (isStatementInForLoop(stmt)) {
			ct = ConstructTrait(ConstructTraitType::CTExpression);
			ct.add(ConstructTraitType::CTSimpleStatement);
			handleStatementWithWrappableCheck(stmt);
			return true;
		}
		return false;
	}

	void handleStatementWithWrappableCheck(clang::Stmt *stmt) {
		ct.add(ConstructTraitType::CTStatement);
		auto matcher = clang::ast_matchers::stmt(clang::ast_matchers::hasParent(clang::ast_matchers::compoundStmt()));
		auto matches = clang::ast_matchers::match(matcher, *stmt, context);
		if (!matches.empty()) {
			InstRO::logIt(InstRO::DEBUG) << "Matched a wrappable statement" << std::endl;
			ct.add(ConstructTraitType::CTWrappableStatement);
		}
	}

	void generateError(clang::Stmt *stmt) {
		InstRO::logIt(InstRO::INFO) << "Skipping statement " << stmt->getStmtClassName() << "(" << stmt << ")" << std::endl;
	}
};

ConstructTrait getConstructTrait(clang::ASTContext &context, clang::Decl *decl) {
	DeclConstructTraitVisitor visitor(context);
	visitor.Visit(decl);
	return visitor.getConstructTrait();
}

ConstructTrait getConstructTrait(clang::ASTContext &context, clang::Stmt *stmt) {
	StmtConstructTraitVisitor visitor(context);
	visitor.Visit(stmt);
	return visitor.getContructTrait();
}
}

clang::ASTContext *ClangConstruct::astContext = nullptr;

clang::ASTContext &ClangConstruct::getASTContext() {
	if (astContext) {
		return *astContext;
	} else {
		InstRO::raise_exception("No ASTContext has been set in ClangConstruct");
	}
}

clang::SourceManager &ClangConstruct::getSourceManager() { return getASTContext().getSourceManager(); }

void ClangConstruct::setASTContext(clang::ASTContext &context) { ClangConstruct::astContext = &context; }

ClangConstruct::ClangConstruct(clang::Decl *decl)
		: Construct(getConstructTrait(getASTContext(), decl)), kind(ConstructKind::CK_Declaration), construct(decl) {}

ClangConstruct::ClangConstruct(clang::Stmt *stmt)
		: Construct(getConstructTrait(getASTContext(), stmt)), kind(ConstructKind::CK_Stmt), construct(stmt) {}

size_t ClangConstruct::getID() const { return reinterpret_cast<size_t>(construct); }

std::string ClangConstruct::toString() const {
	std::string className;

	std::string unparse;
	llvm::raw_string_ostream rso(unparse);

	if (clang::Decl *decl = getAsDecl()) {
		className = decl->getDeclKindName();
	} else if (clang::Stmt *stmt = getAsStmt()) {
		className = stmt->getStmtClassName();
	}

	ConstructTrait ct = getTraits();
	if (ct.is(ConstructTraitType::CTFunction)) {
		if (clang::FunctionDecl *funDecl = llvm::dyn_cast<clang::FunctionDecl>(getAsDecl())) {
			rso << funDecl->getNameAsString();
		} else {
			InstRO::raise_exception("Invalid FunctionDecl");
		}
	} else if (ct.is(ConstructTraitType::CTFileScope)) {
		if (clang::TranslationUnitDecl *translationDecl = llvm::dyn_cast<clang::TranslationUnitDecl>(getAsDecl())) {
			rso << getSourceManager().getFilename(translationDecl->getLocStart());
		} else {
			InstRO::raise_exception("Invalid TranslationUnitDecl");
		}
	} else {
		// unparse statement or decl
		if (clang::Decl *decl = getAsDecl()) {
			decl->print(rso, getASTContext().getPrintingPolicy());
		} else if (clang::Stmt *stmt = getAsStmt()) {
			stmt->printPretty(rso, nullptr, getASTContext().getPrintingPolicy());
		}
	}

	rso.str();
	// remove trailing newline from unparsed construct
	boost::trim_right(unparse);

	return className + " | " + unparse;
}

std::string ClangConstruct::toDotString() const {
	std::string dotString(toString());
	// escape " and \n (e.g. in string literals)
	boost::replace_all(dotString, "\n", "\\n");
	boost::replace_all(dotString, "\"", "\\\"");
	return dotString;
}

std::string ClangConstruct::getIdentifier() const {
	std::string identifier;

	clang::SourceManager &sourceManager = getSourceManager();

	clang::SourceLocation location;
	if (clang::Decl *decl = getAsDecl()) {
		location = decl->getLocStart();
	} else if (clang::Stmt *stmt = getAsStmt()) {
		location = stmt->getLocStart();
	} else {
		InstRO::raise_exception("Unknown type in ClangConstruct");
	}

	identifier =
			(sourceManager.getFilename(location) + ":" + llvm::Twine((sourceManager.getSpellingLineNumber(location)))).str();
	identifier += ":" + std::to_string(sourceManager.getSpellingColumnNumber(location));
	identifier += "--" + specificConstructTraitToString();

	if (clang::Decl *decl = getAsDecl()) {
		if (clang::FunctionDecl *funDecl = llvm::dyn_cast<clang::FunctionDecl>(decl)) {
			if (funDecl->isThisDeclarationADefinition()) {
				identifier += "-" + getFunctionName(funDecl);
			}
		}
	}

	return identifier;
}

std::string ClangConstruct::getFunctionName(clang::FunctionDecl *decl) const {
	std::string buffer;
	llvm::raw_string_ostream rso(buffer);

	rso << "::";
	decl->printQualifiedName(rso);
	clang::FunctionTemplateSpecializationInfo *templateSpecInfo = decl->getTemplateSpecializationInfo();
	if (templateSpecInfo) {
		rso << '<';
		const clang::TemplateArgumentList *argList = templateSpecInfo->TemplateArguments;
		for (unsigned i = 0; i < argList->size(); ++i) {
			argList->get(i).print(getASTContext().getPrintingPolicy(), rso);
			if (i < argList->size() - 1) {
				rso << ',';
			}
		}
		rso << '>';
	}

	return rso.str();
}

clang::Decl *ClangConstruct::getAsDecl() const {
	return (kind == ConstructKind::CK_Declaration) ? static_cast<clang::Decl *>(construct) : nullptr;
}

clang::Stmt *ClangConstruct::getAsStmt() const {
	return (kind == ConstructKind::CK_Stmt) ? static_cast<clang::Stmt *>(construct) : nullptr;
}
