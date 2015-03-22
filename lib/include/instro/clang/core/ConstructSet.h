#ifndef INSTRO_CLANG_CONSTRUCTSET_H
#define INSTRO_CLANG_CONSTRUCTSET_H

#include "instro/core/ConstructSet.h"

#include "clang/AST/Decl.h"

namespace InstRO {
namespace Clang {

/*
 * Helper data structure to have some common base class
 * for Clang Stmt / Decl / Type voodoo
 */
struct ClangConstruct {
	clang::Decl *decl;
	clang::Stmt *stmt;
	clang::Type *type;
};

clang::Decl *getAsDecl(const ClangConstruct &c);
clang::Stmt *getAsStmt(const ClangConstruct &c);
clang::Type *getAsType(const ClangConstruct &c);

class ClangConstructSet : public InstRO::Core::ConstructSet {
 public:
	void put(clang::Decl *decl);
	void put(clang::Stmt *stmt);
	std::vector<ClangConstruct> getConstructSet();
	void selectConstruct(InstRO::Core::Construct *c) override {};
	std::vector<InstRO::Core::Construct*> getConstructs() override {};

 private:
	std::vector<ClangConstruct> constructs;
};


void print(std::ostream &outStream, ClangConstructSet *cs);

}	// Clang
}	// InstRO

#endif
