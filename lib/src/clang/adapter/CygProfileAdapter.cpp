#include "instro/clang/adapter/CygProfileAdapter.h"

InstRO::Clang::CygProfileAdapter::CygProfileAdapter(InstRO::Pass *selector, clang::tooling::Replacements &reps,
																										clang::SourceManager *sm)
		: decidingSelector(selector), sm(sm), replacements(reps), labelCount(0), cygProfFuncPtrName("__instro_func_ptr") {}

void InstRO::Clang::CygProfileAdapter::init() {}

void InstRO::Clang::CygProfileAdapter::execute() { executer->execute(this); }

bool InstRO::Clang::CygProfileAdapter::VisitFunctionDecl(clang::FunctionDecl *decl) {
	if (context == nullptr) {
		std::cerr << "Context NULL, stopping adapter pass." << std::endl;
		return false;
	}
	// The context is in ClangPassImplementation
	sm = &context->getSourceManager();

	// since we are in a Clang Adapter, we cast. I KNOW THIS IS UGLY
	InstRO::Core::ConstructSet *c = getInput(decidingSelector);
	cs = *(reinterpret_cast<InstRO::Clang::ClangConstructSet *>(c));
	
	for (auto &construct : cs.getConstructSet()) {
		if (InstRO::Clang::getAsDecl(construct) == decl) {
			// the function declaration we are visitting has actually been selected
			dispatch(decl);
		}
	}

	labelCount++;
	return true;
}

void InstRO::Clang::CygProfileAdapter::finalize() {}

void InstRO::Clang::CygProfileAdapter::releaseOutput() {}

InstRO::Clang::ClangConstructSet *InstRO::Clang::CygProfileAdapter::getOutput() { return nullptr; }

void InstRO::Clang::CygProfileAdapter::dispatch(clang::Decl *c) {
	std::cout << "Dispatching..." << std::endl;
	clang::CXXMethodDecl *mDef = llvm::dyn_cast<clang::CXXMethodDecl>(c);
	if (mDef != nullptr) {
		transform(sm, mDef);
		return;
	}

	/*
	 * FIXME This guy still cannot handle overloaded functions
	 */
	clang::FunctionDecl *fDef = llvm::dyn_cast<clang::FunctionDecl>(c);
	if (fDef != nullptr) {
		// Inside this case we need to check for the overload
		transform(sm, fDef);
	}
}

void InstRO::Clang::CygProfileAdapter::transform(clang::SourceManager *sm, clang::FunctionDecl *decl) {
	// We are assuming we are only instrumenting function definitions!
	clang::FunctionDecl *fDef = llvm::dyn_cast<clang::FunctionDecl>(decl);

	// Depending on whether the funciton is overloaded we need to generate different strings
	std::cout << "Trying to query the decl context" << std::endl;

	std::string entryReplaceStr = generateFunctionEntry(fDef);
	std::string exitReplaceStr = generateFunctionExit(fDef);

	clang::Stmt *fBodyStmt = fDef->getBody();

	// I assume that a function body always is a compound statement
	clang::CompoundStmt *fBody = llvm::dyn_cast<clang::CompoundStmt>(fBodyStmt);

	instrumentFunctionBody(fBody, entryReplaceStr, exitReplaceStr);
}

bool InstRO::Clang::CygProfileAdapter::isOverloadedFunction(clang::FunctionDecl *decl) {
	auto funcLookUp = decl->getEnclosingNamespaceContext()->lookup(decl->getNameInfo().getName());
	int overloadCount = 0;
	for (auto &res : funcLookUp) {
		clang::FunctionDecl *testDecl = llvm::dyn_cast<clang::FunctionDecl>(res);
		if (testDecl != nullptr) {
			// the found declaration actually is a function declaration
			if (decl->getDeclName() != testDecl->getDeclName()) {
				//				std::cout << "decl: " << decl->getNameAsString() << " testDecl: " << testDecl->getNameAsString() <<
				// std::endl;
				//				std::cout << "Found a different name" << std::endl;
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
					//					std::cout << "Function Type pointer were equal." << std::endl;
				} else {
					//					std::cout << "Function type were different" << std::endl;
					overloadCount++;
				}
			}
		}
		if (overloadCount > 0) {
			std::cout << "The result " << res->getNameAsString() << " was an overload" << std::endl;
		}
	}
	return (overloadCount > 0);
}

void InstRO::Clang::CygProfileAdapter::transform(clang::SourceManager *sm, clang::CXXMethodDecl *decl) {
	clang::CXXMethodDecl *fDef = llvm::dyn_cast<clang::CXXMethodDecl>(decl);

	std::string entryReplaceStr = generateMethodEntry(fDef);
	std::string exitReplaceStr = generateMethodExit(fDef);

	// We are assuming we are only instrumenting function definitions!
	clang::Stmt *fBodyStmt = fDef->getBody();
	// I assume that a function body always is a compound statement
	clang::CompoundStmt *fBody = llvm::dyn_cast<clang::CompoundStmt>(fBodyStmt);

	instrumentFunctionBody(fBody, entryReplaceStr, exitReplaceStr);
}

void InstRO::Clang::CygProfileAdapter::instrumentFunctionBody(clang::CompoundStmt *body, std::string &entryStr,
																															std::string &exitStr) {
	// If the function body is empty, we need to insert calls in reverse order
	if (body->size() == 0) {
		handleEmptyBody(body, entryStr, exitStr);
		return;
	}
	// we are inserting the exit hooks as well
	instrumentReturnStatements(body, entryStr, exitStr);

	// We add the calls to our entry functions
	//	clang::Stmt *startStmt = body;	//*(fBody->body().begin());
	clang::tooling::Replacement repMent(*sm, body->getLBracLoc(), 1, std::string("{" + entryStr));
	replacements.insert(repMent);
}

void InstRO::Clang::CygProfileAdapter::handleEmptyBody(clang::CompoundStmt *body, std::string &entryStr,
																											 std::string &exitStr) {
	/*
	 * If this is an empty body we insert the exit function at the end and replace
	 * the opening bracket with the opening bracket followed by the function call
	 * to the measurement function
	 */
	replacements.insert(clang::tooling::Replacement(*sm, body->getRBracLoc(), 0, exitStr));
	replacements.insert(clang::tooling::Replacement(*sm, body->getLBracLoc(), 1, std::string("{" + entryStr)));
}

void InstRO::Clang::CygProfileAdapter::instrumentReturnStatements(clang::CompoundStmt *body, std::string &entryStr,
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
			replacements.insert(clang::tooling::Replacement(*sm, rSt->getLocStart(), 0, exitStr));
		} else if (st == body->body_back()) {
			// instrument end of function without return stmt
			replacements.insert(clang::tooling::Replacement(*sm, st->getLocStart(), 0, exitStr));
		}
	}
}
void InstRO::Clang::CygProfileAdapter::transformReturnStmt(clang::ReturnStmt *retStmt) {
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
	replacements.insert(clang::tooling::Replacement(*sm, e, iVarName));

	// insert the declaration of the newly created temporary
	replacements.insert(clang::tooling::Replacement(*sm, retStmt->getLocStart(), 0, tVar));
}

std::string InstRO::Clang::CygProfileAdapter::generateFunctionEntry(clang::FunctionDecl *d) {
	if(isOverloadedFunction(d)){
		std::string declStr = generateFunctionPointerDecl(cygProfFuncPtrName, d);
		std::string callStr = generateCallTo("enter", cygProfFuncPtrName);
		return std::string(declStr + callStr);
	} else {
		return generateCallTo("enter", d);
	}
}

std::string InstRO::Clang::CygProfileAdapter::generateFunctionExit(clang::FunctionDecl *d) {
	if(isOverloadedFunction(d)){
		return generateCallTo("exit", cygProfFuncPtrName);
	} else {
		return generateCallTo("exit", d);
	}
}

std::string InstRO::Clang::CygProfileAdapter::generateFunctionPointerDecl(std::string declName,
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

std::string InstRO::Clang::CygProfileAdapter::generateCallTo(std::string fName, std::string newDecl) {
	std::string snippet("__cyg_profile_func_");
	snippet += fName;
	snippet += "((void*) ";
	snippet += newDecl;
	snippet += " , 0);";

	std::cout << "Cyg Profile call: " << snippet << std::endl;

	return snippet;
}

std::string InstRO::Clang::CygProfileAdapter::generateCallTo(std::string fName, clang::FunctionDecl *d) {
	std::string snippet("__cyg_profile_func_");
	snippet += fName;
	snippet += "((void*) ";
	snippet += d->getNameInfo().getName().getAsString();
	snippet += ", 0);";

	std::cout << "Cyg Profile call: " << snippet << std::endl;

	return snippet;
}

std::string InstRO::Clang::CygProfileAdapter::generateMethodEntry(clang::CXXMethodDecl *d) {
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

std::string InstRO::Clang::CygProfileAdapter::generateMethodExit(clang::CXXMethodDecl *d) {
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

bool InstRO::Clang::CygProfileAdapter::retStmtNeedsTransformation(clang::ReturnStmt *st) {
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
