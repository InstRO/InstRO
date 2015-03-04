#include "instro/clang/adapter/CygProfileAdapter.h"

void ::InstRO::Clang::CygProfileAdapter::run(){
	for(auto &construct : cs.getConstructSet()){
		adapt(construct);
	}
}

void ::InstRO::Clang::CygProfileAdapter::adapt(::InstRO::Clang::ClangConstruct c){
	// we have to check whether the returned value is NULL
	clang::Decl *decl = ::InstRO::Clang::getAsDecl(c);
	transform(sm, decl);
}

void ::InstRO::Clang::CygProfileAdapter::transform(clang::SourceManager *sm, clang::Decl *decl){
	assert(decl != NULL);
	// We are assuming we are only instrumenting function definitions!
	clang::FunctionDecl *fDef = llvm::dyn_cast<clang::FunctionDecl>(decl);
	// We add the calls to our entry functions
	std::string entryReplaceStr("__cyg_profile_func_enter((void*) " + fDef->getNameInfo().getName().getAsString() + ", 0)");
	replacements->insert(clang::tooling::Replacement(*sm, fDef->getLocStart(), 0, entryReplaceStr));

	// we are inserting the exit hooks as well
	std::string exitReplaceStr("__cyg_profile_func_exit((void*( " + fDef->getNameInfo().getName().getAsString() + ", 0)");
	clang::Stmt *fBodyStmt = fDef->getBody();
	// I assume that a function body always is a compound statement
	clang::CompoundStmt *fBody = llvm::dyn_cast<clang::CompoundStmt>(fBodyStmt);
	for(auto &st : fBody->body()){
		clang::ReturnStmt *rSt = llvm::dyn_cast<clang::ReturnStmt>(st);
		if(rSt){
			replacements->insert(clang::tooling::Replacement(*sm, rSt->getLocStart(), 0, exitReplaceStr));
		} else if(st == fBody->body_back()){
			replacements->insert(clang::tooling::Replacement(*sm, st->getLocStart(), 0, exitReplaceStr));	
		}
	}
	std::cout << "Transforming declaration" << std::endl;
}
