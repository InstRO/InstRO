#include "instro/clang/pass/adapter/ClangCygProfileAdapter.h"

#include "instro/clang/core/ClangConstruct.h"
#include "instro/utility/Logger.h"

using namespace InstRO::Clang::Adapter;

ClangCygProfileAdapter::ClangCygProfileAdapter(ReplacementsMap &reps, clang::SourceManager *sm)
		: ClangPassImplBase(new InstRO::Clang::VisitingPassExecuter<ClangCygProfileAdapter>()),
			sm(sm),
			replacements(reps),
			labelCount(0),
			cygProfFuncPtrName("__instro_func_ptr") {}

bool ClangCygProfileAdapter::VisitFunctionDecl(clang::FunctionDecl *decl) {
	if (context == nullptr) {
		logIt(ERROR) << "Context NULL, stopping adapter pass." << std::endl;
		return false;
	}
	// The context is in ClangPassImplementation
	sm = &context->getSourceManager();

	const InstRO::Core::ConstructSet *c = getInput(0);
	InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface rcsci(c);

	for (auto &construct : rcsci) {
		if (std::dynamic_pointer_cast<InstRO::Clang::Core::ClangConstruct>(construct)->getAsDecl() == decl) {
			// the function declaration we are visiting has actually been selected
			dispatch(decl);
		}
	}

	labelCount++;
	return true;
}

void ClangCygProfileAdapter::dispatch(clang::Decl *c) {
	logIt(DEBUG) << "Dispatching..." << std::endl;
	clang::CXXMethodDecl *mDef = llvm::dyn_cast<clang::CXXMethodDecl>(c);
	if (mDef != nullptr) {
		transform(sm, mDef);
		return;
	}

	clang::FunctionDecl *fDef = llvm::dyn_cast<clang::FunctionDecl>(c);
	if (fDef != nullptr) {
		// Inside this case we need to check for the overload
		transform(sm, fDef);
	}
}

void ClangCygProfileAdapter::transform(clang::SourceManager *sm, clang::FunctionDecl *decl) {
	// We are assuming we are only instrumenting function definitions!
	clang::FunctionDecl *fDef = llvm::dyn_cast<clang::FunctionDecl>(decl);

	// Depending on whether the funciton is overloaded we need to generate different strings
	logIt(DEBUG) << "Trying to query the decl context" << std::endl;

	std::string entryReplaceStr = generateFunctionEntry(fDef);
	std::string exitReplaceStr = generateFunctionExit(fDef);

	clang::Stmt *fBodyStmt = fDef->getBody();

	// I assume that a function body always is a compound statement
	clang::CompoundStmt *fBody = llvm::dyn_cast<clang::CompoundStmt>(fBodyStmt);

	instrumentFunctionBody(fBody, entryReplaceStr, exitReplaceStr);
}

bool ClangCygProfileAdapter::isOverloadedFunction(clang::FunctionDecl *decl) {
	auto funcLookUp = decl->getEnclosingNamespaceContext()->lookup(decl->getNameInfo().getName());
	int overloadCount = 0;
	for (auto &res : funcLookUp) {
		clang::FunctionDecl *testDecl = llvm::dyn_cast<clang::FunctionDecl>(res);
		if (testDecl != nullptr) {
			// the found declaration actually is a function declaration
			if (decl->getDeclName() != testDecl->getDeclName()) {
				continue;
			}
			clang::QualType qt = testDecl->getType();
			const clang::Type *typePtr = qt.getTypePtr();
			// In clang a function can be of two types
			const clang::FunctionType *fType = llvm::dyn_cast<clang::FunctionType>(typePtr);
			if (fType != nullptr) {
				clang::QualType dqt = decl->getType();
				const clang::Type *dtypePtr = dqt.getTypePtr();
				const clang::FunctionType *dfType = llvm::dyn_cast<clang::FunctionType>(dtypePtr);
				if (dfType && fType == dfType) {
				} else {
					overloadCount++;
				}
			}
		}
		if (overloadCount > 0) {
			logIt(INFO) << "The result " << res->getNameAsString() << " was an overload" << std::endl;
		}
	}
	return (overloadCount > 0);
}

void ClangCygProfileAdapter::transform(clang::SourceManager *sm, clang::CXXMethodDecl *decl) {
	clang::CXXMethodDecl *fDef = llvm::dyn_cast<clang::CXXMethodDecl>(decl);

	std::string entryReplaceStr = generateMethodEntry(fDef);
	std::string exitReplaceStr = generateMethodExit(fDef);

	// We are assuming we are only instrumenting function definitions!
	clang::Stmt *fBodyStmt = fDef->getBody();
	// I assume that a function body always is a compound statement
	clang::CompoundStmt *fBody = llvm::dyn_cast<clang::CompoundStmt>(fBodyStmt);

	instrumentFunctionBody(fBody, entryReplaceStr, exitReplaceStr);
}

void ClangCygProfileAdapter::instrumentFunctionBody(clang::CompoundStmt *body, std::string &entryStr,
																															std::string &exitStr) {
	// If the function body is empty, we need to insert calls in reverse order
	if (body->size() == 0) {
		handleEmptyBody(body, entryStr, exitStr);
		return;
	}
	// we are inserting the exit hooks as well
	instrumentReturnStatements(body, entryStr, exitStr);

	// We add the calls to our entry functions
	clang::tooling::Replacement repMent(*sm, body->getLBracLoc(), 1, std::string("{" + entryStr));
	insertReplacement(repMent);
}

void ClangCygProfileAdapter::handleEmptyBody(clang::CompoundStmt *body, std::string &entryStr,
																											 std::string &exitStr) {
	/*
	 * If this is an empty body we insert the exit function at the end and replace
	 * the opening bracket with the opening bracket followed by the function call
	 * to the measurement function
	 */
	insertReplacement(clang::tooling::Replacement(*sm, body->getRBracLoc(), 0, exitStr));
	insertReplacement(clang::tooling::Replacement(*sm, body->getLBracLoc(), 1, std::string("{" + entryStr)));
}

void ClangCygProfileAdapter::instrumentReturnStatements(clang::CompoundStmt *body, std::string &entryStr,
																																	std::string &exitStr) {
	for (auto &st : body->body()) {
		clang::ReturnStmt *rSt = llvm::dyn_cast<clang::ReturnStmt>(st);
		if (rSt != nullptr) {
			/*
			 * If an expression other than just a literal or a declaration reference we want to transform
			 * the return statement, so that we capture the time it takes to evaluate the expression
			 */
			if (retStmtNeedsTransformation(rSt)) {
				transformReturnStmt(rSt);
			}

			// instrument return statements
			insertReplacement(clang::tooling::Replacement(*sm, rSt->getBeginLoc(), 0, exitStr));
		} else if (st == body->body_back()) {
			// instrument end of function without return stmt
			insertReplacement(clang::tooling::Replacement(*sm, body->getRBracLoc(), 0, exitStr));
		}
	}
}
void ClangCygProfileAdapter::transformReturnStmt(clang::ReturnStmt *retStmt) {
	/*
	 * create temporary variable to store the expression hidden in the return stmt
	 */
	clang::Expr *e = retStmt->getRetValue();
	clang::QualType t = e->getType();

	// clangs style to get the "original string representation" of the expression
	std::string exprStr;
	llvm::raw_string_ostream s(exprStr);
	e->printPretty(s, 0, context->getPrintingPolicy());
	// ---

	std::string iVarName(" __instro_" + std::to_string(reinterpret_cast<unsigned long>(this)));
	std::string tVar(t.getAsString() + iVarName + " = " + s.str() + ";");

	// refer in return statement to newly created variable
	insertReplacement(clang::tooling::Replacement(*sm, e, iVarName));

	// insert the declaration of the newly created temporary
	insertReplacement(clang::tooling::Replacement(*sm, retStmt->getBeginLoc(), 0, tVar));
}

std::string ClangCygProfileAdapter::generateFunctionEntry(clang::FunctionDecl *d) {
	if (isOverloadedFunction(d)) {
		std::string declStr = generateFunctionPointerDecl(cygProfFuncPtrName, d);
		std::string callStr = generateCallTo("enter", cygProfFuncPtrName);
		return std::string(declStr + callStr);
	} else {
		return generateCallTo("enter", d);
	}
}

std::string ClangCygProfileAdapter::generateFunctionExit(clang::FunctionDecl *d) {
	if (isOverloadedFunction(d)) {
		return generateCallTo("exit", cygProfFuncPtrName);
	} else {
		return generateCallTo("exit", d);
	}
}

std::string ClangCygProfileAdapter::generateFunctionPointerDecl(std::string declName,
																																					clang::FunctionDecl *d) {
	std::string funcRefSnippet("");
	// we need to create something like
	// foo_t (* thisFoo) (foo_t1, foo_t2) = foo;
	funcRefSnippet += d->getReturnType().getAsString();
	funcRefSnippet += " (* ";
	funcRefSnippet += declName;
	funcRefSnippet += ") (";
	for (int i = 0; i < d->parameters().size() - 1; ++i) {
		funcRefSnippet += d->getParamDecl(i)->getType().getAsString() + ", ";
	}
	funcRefSnippet += d->getParamDecl(d->parameters().size() - 1)->getType().getAsString();
	funcRefSnippet += ") = ";
	funcRefSnippet += d->getNameInfo().getName().getAsString();
	funcRefSnippet += ";";
	return funcRefSnippet;
}

std::string ClangCygProfileAdapter::generateCallTo(std::string fName, std::string newDecl) {
	std::string snippet("__cyg_profile_func_");
	snippet += fName;
	snippet += "((void*) ";
	snippet += newDecl;
	snippet += " , 0);";

	logIt(DEBUG) << "Cyg Profile call: " << snippet << std::endl;

	return snippet;
}

std::string ClangCygProfileAdapter::generateCallTo(std::string fName, clang::FunctionDecl *d) {
	std::string snippet("__cyg_profile_func_");
	snippet += fName;
	snippet += "((void*) ";
	snippet += d->getNameInfo().getName().getAsString();
	snippet += ", 0);";

	logIt(DEBUG) << "Cyg Profile call: " << snippet << std::endl;

	return snippet;
}

std::string ClangCygProfileAdapter::generateMethodEntry(clang::CXXMethodDecl *d) {
	/*
	 * For member function we insert inline assembly since, we are not allowed to take a function pointer
	 * for member functions
	 */
	std::string asmString("__asm__(\"_");
	asmString += std::to_string(labelCount);
	asmString +=
			"Laf:\n\tmovq %rdi, -8(%rbp)\n\tmovq 8(%rbp), %rax\n\tmovq %rax, "
			"%rsi\n\tmovl $_";
	asmString += std::to_string(labelCount);
	asmString += "Laf, %edi\ncall __cyg_profile_func_enter\n\");";
	return asmString;
}

std::string ClangCygProfileAdapter::generateMethodExit(clang::CXXMethodDecl *d) {
	/*
	 * For member function we insert inline assembly since, we are not allowed to take a function pointer
	 * for member functions
	 */
	std::string asmString("__asm__(\"_");
	asmString +=
			"\tmovq %rdi, -8(%rbp)\n\tmovq 8(%rbp), %rax\n\tmovq %rax, %rsi\n\tmovl "
			"$_";
	asmString += std::to_string(labelCount);
	asmString += "Laf, %edi\ncall __cyg_profile_func_exit\n\");";
	return asmString;
}

bool ClangCygProfileAdapter::retStmtNeedsTransformation(clang::ReturnStmt *st) {
	/*
	 * We want to check whether the expression within the Return statement is of
	 * type
	 * CXXBoolLiteralExpr,
	 * FloatingLiteral
	 * IntegerLiteral
	 * ImaginaryLiteral
	 * StringLiteral
	 * DeclRefExpr
	 * For all other expression types we need to transform the return statement
	 */
	clang::Expr *retExpr = st->getRetValue();
	if (llvm::dyn_cast<clang::CXXBoolLiteralExpr>(retExpr) != nullptr) {
		return false;
	}
	if (llvm::dyn_cast<clang::FloatingLiteral>(retExpr) != nullptr) {
		return false;
	}
	if (llvm::dyn_cast<clang::IntegerLiteral>(retExpr) != nullptr) {
		return false;
	}
	if (llvm::dyn_cast<clang::ImaginaryLiteral>(retExpr) != nullptr) {
		return false;
	}
	if (llvm::dyn_cast<clang::StringLiteral>(retExpr) != nullptr) {
		return false;
	}
	if (llvm::dyn_cast<clang::DeclRefExpr>(retExpr) != nullptr) {
		return false;
	}
	return true;
}

void ClangCygProfileAdapter::insertReplacement(clang::tooling::Replacement rep) {
	auto file = rep.getFilePath().str();
	auto& reps = replacements[file];
	reps.add(rep);
}

