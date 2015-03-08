#include "instro/clang/core/ConstructSet.h"

clang::Decl * ::InstRO::Clang::getAsDecl(
		const ::InstRO::Clang::ClangConstruct &c) {
	return c.decl;
}
clang::Stmt * ::InstRO::Clang::getAsStmt(
		const ::InstRO::Clang::ClangConstruct &c) {
	return c.stmt;
}
clang::Type * ::InstRO::Clang::getAsType(
		const ::InstRO::Clang::ClangConstruct &c) {
	return c.type;
}

void InstRO::Clang::ClangConstructSet::put(clang::Decl *decl) {
	InstRO::Clang::ClangConstruct c;
	c.decl = decl;
	constructs.push_back(c);
}
void InstRO::Clang::ClangConstructSet::put(clang::Stmt *stmt) {
	InstRO::Clang::ClangConstruct c;
	c.stmt = stmt;
	constructs.push_back(c);
}

std::vector<InstRO::Clang::ClangConstruct>
InstRO::Clang::ClangConstructSet::getConstructSet() {
	return constructs;
}

