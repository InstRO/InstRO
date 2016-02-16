#include "instro/clang/core/ClangConstruct.h"

#include "instro/utility/exception.h"
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

class DeclConstructTraitVisitor : public clang::DeclVisitor<DeclConstructTraitVisitor> {
 public:
	DeclConstructTraitVisitor() : ct(ConstructTraitType::CTNoTraits) {}
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

	void VisitDecl(clang::Decl *decl) { generateError(decl); }

 private:
	ConstructTrait ct;

	void generateError(clang::Decl *decl) {
		InstRO::logIt(InstRO::INFO) << "Skipping declaration " << decl->getDeclKindName() << "(" << decl << ")"
																<< std::endl;
	}
};

class StmtConstructTraitVisitor : public clang::StmtVisitor<StmtConstructTraitVisitor> {
 public:
	StmtConstructTraitVisitor(clang::ASTContext &context) : context(context), ct(ConstructTraitType::CTNoTraits) {}
	ConstructTrait getContructTrait() { return ct; }

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
		// ignore the top level compound statement of a function
		auto matcher = clang::ast_matchers::stmt(clang::ast_matchers::hasParent(clang::ast_matchers::functionDecl()));
		auto matches = clang::ast_matchers::match(matcher, *stmt, context);
		if (matches.empty()) {
			ct = ConstructTrait(ConstructTraitType::CTScopeStatement);
			handleStatementWithWrappableCheck(stmt);
		} else {
			generateError(stmt);
		}
	}

	void VisitDeclStmt(clang::DeclStmt *stmt) {
		// check whether all declarations are initialized variables
		bool allInitVarDecls = false;
		for (clang::Decl *decl : stmt->getDeclGroup()) {
			if (clang::VarDecl *varDecl = llvm::dyn_cast<clang::VarDecl>(decl)) {
				if (varDecl->hasInit()) {
					allInitVarDecls = true;
				} else {
					allInitVarDecls = false;
				}
			} else {
				allInitVarDecls = false;
			}
		}

		if (allInitVarDecls) {
			ct = ConstructTrait(ConstructTraitType::CTSimpleStatement);
			handleStatementWithWrappableCheck(stmt);
		} else {
			generateError(stmt);
		}
	}

	void VisitExpr(clang::Expr *stmt) {
		// in Clang every Expr is also a Stmt, therefore Expressions might also be SimpleStatements

		clang::SourceLocation location = stmt->getLocStart();
		const clang::SourceManager &sm = context.getSourceManager();
		std::string strLoc = "(" + std::to_string(sm.getSpellingLineNumber(location)) + "," +
												 std::to_string(sm.getSpellingColumnNumber(location)) + ")";

		bool isNotAStatement = false;
		auto parents = context.getParents(*stmt);
		if (parents.size() == 1) {
			if (const clang::Stmt *parent = parents.front().get<clang::Stmt>()) {
				isNotAStatement = llvm::isa<clang::Expr>(*parent);
				if (!isNotAStatement) {
					if (const clang::IfStmt *ifStmt = llvm::dyn_cast<clang::IfStmt>(parent)) {
						// the conditional expression of a if statement is no statement
						// the 'then' and 'else' statements can be expressions, so we cannot just check the type of the parent
						isNotAStatement = stmt == ifStmt->getCond();
					} else if (const clang::ForStmt *forStmt = llvm::dyn_cast<clang::ForStmt>(parent)) {
						// the 'init' and 'cond' parts are considered to be statements according to the C++ grammar, the 'inc' part
						// is just an expression
						isNotAStatement = stmt == forStmt->getInc();
					} else if (const clang::DoStmt *doStmt = llvm::dyn_cast<clang::DoStmt>(parent)) {
						isNotAStatement = stmt == doStmt->getCond();
					} else if (const clang::WhileStmt *whileStmt = llvm::dyn_cast<clang::WhileStmt>(parent)) {
						isNotAStatement = stmt == whileStmt->getCond();
					}
				}
			} else if (const clang::Decl *parent = parents.front().get<clang::Decl>()) {
				isNotAStatement = true;
			}
		}

		std::string nodeTypeName = stmt->getStmtClassName();
		ct = ConstructTrait(ConstructTraitType::CTExpression);
		if (!isNotAStatement) {
			InstRO::logIt(InstRO::DEBUG) << nodeTypeName << " is a SimpleStatement " << strLoc << std::endl;
			ct.add(ConstructTraitType::CTSimpleStatement);
			handleStatementWithWrappableCheck(stmt);
		}
	}

	void VisitCXXBoolLiteralExpr(clang::CXXBoolLiteralExpr *stmt) {
		// literals are not considered interesting
		generateError(stmt);
	}

	void VisitIntegerLiteral(clang::IntegerLiteral *stmt) {
		// literals are not considered interesting
		generateError(stmt);
	}

	void VisitFloatingLiteral(clang::FloatingLiteral *stmt) {
		// literals are not considered interesting
		generateError(stmt);
	}

	void VisitCharacterLiteral(clang::CharacterLiteral *stmt) {
		// literals are not considered interesting
		generateError(stmt);
	}

	void VisitStringLiteral(clang::StringLiteral *stmt) {
		// literals are not considered interesting
		generateError(stmt);
	}

	void VisitCXXNullPtrLiteralExpr(clang::CXXNullPtrLiteralExpr *stmt) {
		// literals are not considered interesting
		generateError(stmt);
	}

	void VisitDeclRefExpr(clang::DeclRefExpr *stmt) {
		// references to declarations (variables) are not considered interesting
		generateError(stmt);
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

	void VisitCXXOperatorCallExpr(clang::CXXOperatorCallExpr *stmt) {
		ct = ConstructTrait(ConstructTraitType::CTSimpleStatement);
		handleStatementWithWrappableCheck(stmt);
	}

	void VisitReturnStmt(clang::ReturnStmt *stmt) {
		ct = ConstructTrait(ConstructTraitType::CTSimpleStatement);
		ct.add(ConstructTraitType::CTStatement);
		// return statements are not wrappable
	}

	void VisitStmt(clang::Stmt *stmt) { generateError(stmt); }

 private:
	clang::ASTContext &context;
	ConstructTrait ct;

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

ConstructTrait getConstructTrait(clang::Decl *decl) {
	DeclConstructTraitVisitor visitor;
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
		: Construct(getConstructTrait(decl)), kind(ConstructKind::CK_Declaration), construct(decl) {}

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
		rso << " < ";
		const clang::TemplateArgumentList *argList = templateSpecInfo->TemplateArguments;
		for (unsigned i = 0; i < argList->size(); ++i) {
			argList->get(i).print(getASTContext().getPrintingPolicy(), rso);
			if (i < argList->size() - 1) {
				rso << ',';
			}
			rso << ' ';
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
