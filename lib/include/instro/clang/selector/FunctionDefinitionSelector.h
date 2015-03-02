#ifndef INSTRO_CLANG_FUNCTIONDEFINITIONSELECTOR_H
#define INSTRO_CLANG_FUNCTIONDEFINITIONSELECTOR_H

// Clang related includes
#include "clang/AST/RecursiveASTVisitor.h"

// InstRO includes
#include "instro/core/Pass.h"
#include "instro/clang/core/ConstructSet.h"

namespace InstRO {
namespace Clang {
class FunctionDefinitionSelector
		: public ::InstRO::Pass,
			public clang::RecursiveASTVisitor<
					::InstRO::Clang::FunctionDefinitionSelector> {
 public:
	FunctionDefinitionSelector();
	bool VisitFunctionDecl(clang::FunctionDecl *fDecl);

 private:
	ClangConstructSet cs;
};
}	// Clang
}	// InstRO
#endif
