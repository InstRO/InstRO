#include "instro/clang/core/ConstructSet.h"

clang::Decl *InstRO::Clang::getAsDecl(const ::InstRO::Clang::ClangConstruct &c) { return c.decl; }
clang::Stmt *InstRO::Clang::getAsStmt(const ::InstRO::Clang::ClangConstruct &c) { return c.stmt; }
clang::Type *InstRO::Clang::getAsType(const ::InstRO::Clang::ClangConstruct &c) { return c.type; }

void InstRO::Clang::print(std::ostream &outStream, InstRO::Clang::ClangConstructSet *cs) {
	outStream << "Printing ConstructSet " << cs << "\n";
	for (auto &c : cs->getConstructSet()) {
		clang::Stmt *stmt = getAsStmt(c);
		// To pretty print a clang::Whatever, we need the ASTContext...
		outStream << &c << "\n";
	}
	outStream << std::endl;
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

std::vector<InstRO::Clang::ClangConstruct> InstRO::Clang::ClangConstructSet::getConstructSet() { return constructs; }

