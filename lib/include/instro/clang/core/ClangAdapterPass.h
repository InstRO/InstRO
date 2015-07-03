#ifndef INSTRO_CLANG_CORE_CLANGADAPTERPASS_H
#define INSTRO_CLANG_CORE_CLANGADAPTERPASS_H

#include <iostream>
#include <cassert>

#include "clang/AST/ASTContext.h"

#include "instro/core/PassImplementation.h"
#include "instro/clang/core/ClangPassExecuter.h"

namespace InstRO {
namespace Clang {
namespace PassManagement {
class ClangPassExecuter;
}

namespace Core {
/*
 * This is to have a common base class for all passes which require information
 * from the ASTContext
 */
class ClangPassImplementation : public InstRO::PassImplementation,
																public clang::RecursiveASTVisitor<ClangPassImplementation> {
 public:

	 ClangPassImplementation() : InstRO::PassImplementation(ChannelConfiguration(ChannelConfiguration::PassInfoTuple(nullptr, 21, 42))){};

	void setASTContext(clang::ASTContext *context) {
		std::cout << "Setting ASTContext" << std::endl;
		this->context = context;
		std::cout << "done." << std::endl;
	};

	void execute() override;
	virtual void exec(){};

	void setPassExecuter(InstRO::Clang::PassManagement::ClangPassExecuter *executer) { this->executer = executer; }

	virtual bool VisitFunctionDecl(clang::FunctionDecl *decl) = 0;

 protected:
	clang::ASTContext *context;
	InstRO::Clang::PassManagement::ClangPassExecuter *executer;
};
}
}
}
#endif
