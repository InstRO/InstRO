#ifndef INSTRO_CLANG_CORE_CLANGADAPTERPASS_H
#define INSTRO_CLANG_CORE_CLANGADAPTERPASS_H

#include <iostream>

#include "clang/AST/ASTContext.h"

#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Clang {
namespace Core {

/*
 * This is to have a common base class for all passes which require information
 * from the ASTContext
 */
class ClangPassImplementation
		: public InstRO::PassImplementation,
			public clang::RecursiveASTVisitor<ClangPassImplementation> {
 public:
	void setASTContext(clang::ASTContext *context) {
		std::cout << "Setting ASTContext" << std::endl;
		this->context = context;
	};

	virtual bool VisitFunctionDecl(clang::FunctionDecl *decl) = 0;

 protected:
	clang::ASTContext *context;
};

}
}
}

#endif
