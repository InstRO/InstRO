#include "instro/clang/core/ConstructSet.h"

clang::Decl * ::InstRO::Clang::getAsDecl(const ::InstRO::Clang::ClangConstruct &c){
	return c.decl;
}
clang::Stmt * ::InstRO::Clang::getAsStmt(const ::InstRO::Clang::ClangConstruct &c){
	return c.stmt;
}
clang::Type * ::InstRO::Clang::getAsType(const ::InstRO::Clang::ClangConstruct &c){
	return c.type;
}

void InstRO::Clang::ClangConstructSet::put(clang::Decl *decl){}
void InstRO::Clang::ClangConstructSet::put(clang::Stmt *stmt){}
std::vector<InstRO::Clang::ClangConstruct> InstRO::Clang::ClangConstructSet::getConstructSet(){}


