#ifndef INSTRO_CLANG_CORE_PASSMANAGEMENT_PASSEXECUTOR_H
#define INSTRO_CLANG_CORE_PASSMANAGEMENT_PASSEXECUTOR_H

#include <iostream>

#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"

#include "instro/core/PassImplementation.h"
#include "instro/core/PassExecuter.h"

#include "instro/clang/core/ClangAdapterPass.h"

namespace InstRO {
namespace Clang {
namespace Core {
namespace PassManagement {
/*
 * This is the Clang specialization of the delegate object.
 * The ASTConsumer needs to set the ASTContext before this executer is ready to
 * execute.
 * This instance first sets the SourceManager in all InstRO::Clang::Pass (?) or
 * InstRO::Clang::AdapterPass instances it is invoked on
 */
class ClangPassExecuter : public InstRO::Core::PassManagement::PassExecuter {
 public:
	ClangPassExecuter(clang::ASTContext *context);
	void execute(InstRO::PassImplementation *pass);
	void setASTContext(clang::ASTContext *context);

 private:
	clang::ASTContext *context;
};
}
}
}
}

#endif
