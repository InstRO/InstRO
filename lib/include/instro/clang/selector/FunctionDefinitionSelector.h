#ifndef INSTRO_CLANG_FUNCTIONDEFINITIONSELECTOR_H
#define INSTRO_CLANG_FUNCTIONDEFINITIONSELECTOR_H

// Clang related includes
#include "clang/AST/RecursiveASTVisitor.h"

// InstRO includes
#include "instro/clang/core/ClangPassImplBase.h"
#include "instro/clang/core/ConstructSet.h"

namespace InstRO {
namespace Clang {

/*
 * A Selector, which selects all function defintions in an AST
 */
class FunctionDefinitionSelector : public InstRO::Clang::ClangPassImplBase<FunctionDefinitionSelector> {
 public:
	FunctionDefinitionSelector();
	bool VisitFunctionDecl(clang::FunctionDecl *fDecl);

	void init(){};
//	void execute() { executer->execute(this); };
	void finalize(){};
	void releaseOutput(){};
	InstRO::Clang::ClangConstructSet *getOutput();

 private:
	ClangConstructSet cs;
};
}	// Clang
}	// InstRO
#endif
