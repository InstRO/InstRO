#include "instro/clang/adapter/CygProfileAdapter.h"

InstRO::Clang::CygProfileAdapter::CygProfileAdapter(InstRO::Pass *selector, clang::tooling::Replacements &reps,
																										clang::SourceManager *sm)
		: decidingSelector(selector), sm(sm), replacements(reps) {}

void InstRO::Clang::CygProfileAdapter::init() {}

void InstRO::Clang::CygProfileAdapter::execute() {
	std::cout << "Executing CygProfileAdapter" << std::endl;
	if (context == nullptr) std::cerr << "Context NULL" << std::endl;
	sm = &context->getSourceManager();
	
	InstRO::Core::ConstructSet *c = getInput(decidingSelector);
	// since we are in a Clang Adapter, we cast. I KNOW THIS IS UGLY
	cs = *(reinterpret_cast<InstRO::Clang::ClangConstructSet *>(c));
	for (auto &construct : cs.getConstructSet()) {
		std::cout << "handling construct" << std::endl;
		adapt(construct);
	}
}

void InstRO::Clang::CygProfileAdapter::finalize() {}

void InstRO::Clang::CygProfileAdapter::releaseOutput() {}

InstRO::Clang::ClangConstructSet *
InstRO::Clang::CygProfileAdapter::getOutput() {
	return nullptr;
}

void InstRO::Clang::CygProfileAdapter::adapt(
		::InstRO::Clang::ClangConstruct c) {
	std::cout << "Get the construct from the clang construct set" << std::endl;
	// we have to check whether the returned value is NULL
	clang::Decl *decl = InstRO::Clang::getAsDecl(c);
	transform(sm, decl);
}

void InstRO::Clang::CygProfileAdapter::transform(clang::SourceManager *sm,
																								 clang::Decl *decl) {
	assert(decl != NULL);
	decl->dump();
	// We are assuming we are only instrumenting function definitions!
	clang::FunctionDecl *fDef = llvm::dyn_cast<clang::FunctionDecl>(decl);
	clang::Stmt *fBodyStmt = fDef->getBody();
	// I assume that a function body always is a compound statement
	clang::CompoundStmt *fBody = llvm::dyn_cast<clang::CompoundStmt>(fBodyStmt);
	// We add the calls to our entry functions
	std::string entryReplaceStr("__cyg_profile_func_enter((void*) " +
															fDef->getNameInfo().getName().getAsString() +
															", 0);");
	clang::Stmt *startStmt = *(fBody->body().begin());
	clang::tooling::Replacement repMent(*sm, startStmt->getLocStart(), 0, entryReplaceStr);
	replacements.insert(repMent);
	std::cout << "replacement: " << repMent.toString() << std::endl;
	// we are inserting the exit hooks as well
	std::string exitReplaceStr("__cyg_profile_func_exit((void*( " +
														 fDef->getNameInfo().getName().getAsString() +
														 ", 0);");
	for (auto &st : fBody->body()) {
		clang::ReturnStmt *rSt = llvm::dyn_cast<clang::ReturnStmt>(st);
		if (rSt) {
			// instrument return statements
			replacements.insert(clang::tooling::Replacement(*sm, rSt->getLocStart(),
																											 0, exitReplaceStr));
		} else if (st == fBody->body_back()) {
			// instrument end of function without return stmt
			replacements.insert(clang::tooling::Replacement(*sm, st->getLocStart(),
																											 0, exitReplaceStr));
		}
	}
	std::cout << "Transforming declaration" << std::endl;
}
