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
namespace PassManagement {

class ClangPassExecuter : public InstRO::PassManagement::PassExecuter {
 public:
	ClangPassExecuter(clang::ASTContext *context);
	void setASTContext(clang::ASTContext *context);

	virtual void execute(InstRO::Core::PassImplementation *pass) = 0;

 protected:
	clang::ASTContext *context;
};
/*
 * This is the Clang specialization of the delegate object.
 * The ASTConsumer needs to set the ASTContext before this executer is ready to
 * execute.
 * It assumes all passes it is invoked on are
 * InstRO::Clang::Core::ClangPassImplementation
 * It first sets the ASTContext in those classes. It then invokes the traversal
 * mechanism.
 */
class VisitingClangPassExecuter : public InstRO::Clang::PassManagement::ClangPassExecuter {
 public:
	VisitingClangPassExecuter(clang::ASTContext *context);
	/**
	 * This function expects an InstRO::Clang::Core::ClangPassImplementation!
	 * It does a reinterpret_cast. If any one has ideas how we can propagate the
	 * type information fot that from the factory all the way to the executer
	 * without templatizing everything I am very interested in the ideas.
	 */
	void execute(InstRO::Core::PassImplementation *pass) override;

 private:
	int counter;
};

class NonVisitingClangPassExecuter : public InstRO::Clang::PassManagement::ClangPassExecuter {
 public:
	NonVisitingClangPassExecuter(clang::ASTContext *context);
	void execute(InstRO::Core::PassImplementation *pass) override;
};

}	// PassManagement
}	// Clang
}	// IsntRO

#endif
