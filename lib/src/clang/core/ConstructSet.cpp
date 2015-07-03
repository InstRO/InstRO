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

void InstRO::Clang::print(std::ostream &outStream, InstRO::Clang::ClangConstructSet *cs,
													clang::ASTContext *astContext) {
	outStream << "Printing ConstructSet " << cs << "\n";
	for (auto &c : cs->getConstructSet()) {
		clang::Decl *decl = getAsDecl(c);
		if (decl != nullptr) {
			clang::NamedDecl *nDecl = llvm::dyn_cast<clang::NamedDecl>(decl);
			if (nDecl) {
				auto mc = astContext->createMangleContext();
				std::string str;
				llvm::raw_string_ostream s(str);
				mc->mangleName(nDecl, s);
				outStream << nDecl->getNameAsString() << " :: " << s.str() << "\n";
			}
		} else if (getAsStmt(c) != nullptr) {
			clang::Stmt *stmt = getAsStmt(c);
			std::string str;
			llvm::raw_string_ostream s(str);
			stmt->printPretty(s, 0, astContext->getPrintingPolicy());
			// To pretty print a clang::Whatever, we need the ASTContext...
			outStream << s.str() << "\n";
		}
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
