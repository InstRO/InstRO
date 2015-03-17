#include "instro/clang/adapter/CygProfileAdapter.h"

InstRO::Clang::CygProfileAdapter::CygProfileAdapter(
		InstRO::Pass *selector, clang::tooling::Replacements &reps,
		clang::SourceManager *sm)
		: decidingSelector(selector), sm(sm), replacements(reps), labelCount(0) {}

void InstRO::Clang::CygProfileAdapter::init() {}

void InstRO::Clang::CygProfileAdapter::execute() { executer->execute(this); }

bool InstRO::Clang::CygProfileAdapter::VisitFunctionDecl(
		clang::FunctionDecl *decl) {
	std::cout << "Executing CygProfileAdapter" << std::endl;

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
	return true;
}

void InstRO::Clang::CygProfileAdapter::finalize() {}

void InstRO::Clang::CygProfileAdapter::releaseOutput() {}

InstRO::Clang::ClangConstructSet *
InstRO::Clang::CygProfileAdapter::getOutput() {
	return nullptr;
}

void InstRO::Clang::CygProfileAdapter::dispatch(clang::Decl *c) {
	std::cout << "Dispatching" << std::endl;
	// Depending on the actual type we dispatch the clang::Decl to the correct
	// transform function
	clang::CXXMethodDecl *mDef = llvm::dyn_cast<clang::CXXMethodDecl>(c);
	if (mDef != nullptr) {
		transform(sm, mDef);
		return;
	}

	// This is a function
	clang::FunctionDecl *fDef = llvm::dyn_cast<clang::FunctionDecl>(c);
	if (fDef != nullptr) {
		transform(sm, fDef);
	}
}

void InstRO::Clang::CygProfileAdapter::transform(clang::SourceManager *sm,
																								 clang::FunctionDecl *decl) {
	std::cout << "Instance of function decl" << std::endl;
	// We are assuming we are only instrumenting function definitions!
	clang::FunctionDecl *fDef = llvm::dyn_cast<clang::FunctionDecl>(decl);

	std::string entryReplaceStr = generateFunctionEntry(fDef);
	std::string exitReplaceStr = generateFunctionExit(fDef);

	clang::Stmt *fBodyStmt = fDef->getBody();
	// I assume that a function body always is a compound statement
	clang::CompoundStmt *fBody = llvm::dyn_cast<clang::CompoundStmt>(fBodyStmt);

	instrumentFunctionBody(fBody, entryReplaceStr, exitReplaceStr);

	std::cout << "Transforming declaration" << std::endl;
}

void InstRO::Clang::CygProfileAdapter::transform(clang::SourceManager *sm,
																								 clang::CXXMethodDecl *decl) {
	std::cout << "Instance of cxxmethod decl" << std::endl;
	clang::CXXMethodDecl *fDef = llvm::dyn_cast<clang::CXXMethodDecl>(decl);

	std::string entryReplaceStr = generateMethodEntry(fDef);
	std::string exitReplaceStr = generateMethodExit(fDef);

	// We are assuming we are only instrumenting function definitions!
	clang::Stmt *fBodyStmt = fDef->getBody();
	// I assume that a function body always is a compound statement
	clang::CompoundStmt *fBody = llvm::dyn_cast<clang::CompoundStmt>(fBodyStmt);

	instrumentFunctionBody(fBody, entryReplaceStr, exitReplaceStr);

	std::cout << "Transforming declaration" << std::endl;
}

void InstRO::Clang::CygProfileAdapter::instrumentFunctionBody(
		clang::CompoundStmt *body, std::string &entryStr, std::string &exitStr) {
	// If the function body is empty, we need to insert calls in reverse order
	if (body->size() == 0) {
		handleEmptyBody(body, entryStr, exitStr);
		return;
	}
	// We add the calls to our entry functions
	clang::Stmt *startStmt = body->body_front();	//*(fBody->body().begin());
	clang::tooling::Replacement repMent(*sm, startStmt->getLocStart(), 1,
																			entryStr);
	replacements.insert(repMent);
	std::cout << "replacement: " << repMent.toString() << std::endl;

	// we are inserting the exit hooks as well
	instrumentReturnStatements(body, entryStr, exitStr);
}

void InstRO::Clang::CygProfileAdapter::handleEmptyBody(
		clang::CompoundStmt *body, std::string &entryStr, std::string &exitStr) {
	std::cout << "Determined size was zero" << std::endl;
	replacements.insert(
			clang::tooling::Replacement(*sm, body->getRBracLoc(), 0, exitStr));
	replacements.insert(
			clang::tooling::Replacement(*sm, body->getLBracLoc(), 1, entryStr));
}

void InstRO::Clang::CygProfileAdapter::instrumentReturnStatements(
		clang::CompoundStmt *body, std::string &entryStr, std::string &exitStr) {
	for (auto &st : body->body()) {
		clang::ReturnStmt *rSt = llvm::dyn_cast<clang::ReturnStmt>(st);
		if (rSt) {
			// instrument return statements
			replacements.insert(
					clang::tooling::Replacement(*sm, rSt->getLocStart(), 0, exitStr));
		} else if (st == body->body_back()) {
			// instrument end of function without return stmt
			replacements.insert(
					clang::tooling::Replacement(*sm, st->getLocStart(), 0, exitStr));
		}
	}
}

std::string InstRO::Clang::CygProfileAdapter::generateFunctionEntry(
		clang::FunctionDecl *d) {
	return std::string("{__cyg_profile_func_enter((void*) " +
										 d->getNameInfo().getName().getAsString() + ", 0);");
}

std::string InstRO::Clang::CygProfileAdapter::generateFunctionExit(
		clang::FunctionDecl *d) {
	return std::string("__cyg_profile_func_exit((void*) " +
										 d->getNameInfo().getName().getAsString() + ", 0);");
}

std::string InstRO::Clang::CygProfileAdapter::generateMethodEntry(
		clang::CXXMethodDecl *d) {
	std::string asmString("{__asm__(\"_");
	asmString += std::to_string(labelCount);
	asmString +=
			"Laf:\n\tmovq %rdi, -8(%rbp)\n\tmovq 8(%rbp), %rax\n\tmovq %rax, "
			"%rsi\n\tmovl $_";
	asmString += std::to_string(labelCount);
	asmString += "Laf, %edi\ncall __cyg_profile_func_enter\n\");";
	return asmString;
}

std::string InstRO::Clang::CygProfileAdapter::generateMethodExit(
		clang::CXXMethodDecl *d) {
	std::string asmString("__asm__(\"_");
	asmString +=
			"\tmovq %rdi, -8(%rbp)\n\tmovq 8(%rbp), %rax\n\tmovq %rax, %rsi\n\tmovl "
			"$_";
	asmString += std::to_string(labelCount);
	asmString += "Laf, %edi\ncall __cyg_profile_func_exit\n\");";
	return asmString;
}

