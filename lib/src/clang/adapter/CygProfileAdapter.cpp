#include "instro/clang/adapter/CygProfileAdapter.h"

InstRO::Clang::CygProfileAdapter::CygProfileAdapter(InstRO::Pass *selector,
																										clang::SourceManager *sm)
		: decidingSelector(selector), sm(sm), replacements(NULL) {}

void InstRO::Clang::CygProfileAdapter::init() {}

void InstRO::Clang::CygProfileAdapter::execute() {
	std::cout << "Executing CygProfileAdapter" << std::endl;
	if (context == nullptr) std::cerr << "Context NULL" << std::endl;
	sm = &context->getSourceManager();
	for (auto &construct : cs.getConstructSet()) {
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
	// We add the calls to our entry functions
	std::string entryReplaceStr("__cyg_profile_func_enter((void*) " +
															fDef->getNameInfo().getName().getAsString() +
															", 0)");
	replacements->insert(clang::tooling::Replacement(*sm, fDef->getLocStart(), 0,
																									 entryReplaceStr));

	// we are inserting the exit hooks as well
	std::string exitReplaceStr("__cyg_profile_func_exit((void*( " +
														 fDef->getNameInfo().getName().getAsString() +
														 ", 0)");
	clang::Stmt *fBodyStmt = fDef->getBody();
	// I assume that a function body always is a compound statement
	clang::CompoundStmt *fBody = llvm::dyn_cast<clang::CompoundStmt>(fBodyStmt);
	for (auto &st : fBody->body()) {
		clang::ReturnStmt *rSt = llvm::dyn_cast<clang::ReturnStmt>(st);
		if (rSt) {
			replacements->insert(clang::tooling::Replacement(*sm, rSt->getLocStart(),
																											 0, exitReplaceStr));
		} else if (st == fBody->body_back()) {
			replacements->insert(clang::tooling::Replacement(*sm, st->getLocStart(),
																											 0, exitReplaceStr));
		}
	}
	std::cout << "Transforming declaration" << std::endl;
}
