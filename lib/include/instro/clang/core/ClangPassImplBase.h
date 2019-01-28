#ifndef INSTRO_CLANG_CORE_CLANGADAPTERPASS_H
#define INSTRO_CLANG_CORE_CLANGADAPTERPASS_H

#include <cassert>
#include <iostream>
#include <memory>

#include "clang/AST/ASTContext.h"

#include "ClangPassExecuter.h"
#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Clang {

/**
 * Interface to set the clang::ASTContext afterwards
 */
class ASTContextProvider {
 public:
	void setASTContext(clang::ASTContext* ctx) { context = ctx; }

	clang::ASTContext* getASTContext() { return context; }

 protected:
	clang::ASTContext* context;
};

/*
 * This is to have a common base class for all passes which need to be executed
 * using an executer object. The class takes ownership of the Executer object.
 * Executer objects are captured into a std::unique_ptr.
 */
template <typename T>
class ClangPassImplBase : public InstRO::Clang::ASTContextProvider,
													public InstRO::Core::PassImplementation,
													public clang::RecursiveASTVisitor<T> {
 public:
	ClangPassImplBase(PassExecuter<T>* pexec) : InstRO::Core::PassImplementation(), executer(pexec) {}
	ClangPassImplBase() = delete;

	virtual void execute() final override {
		assert(executer != nullptr);
		executer->setASTContext(context);
		executer->execute(static_cast<T*>(this));
	};

	virtual void exec(){};

	bool shouldVisitTemplateInstantiations() const { return true; }

	bool shouldVisitImplicitCode() const { return true; }

 protected:
	std::unique_ptr<PassExecuter<T>> executer;
};
}	// namespace Clang
}	// namespace InstRO
#endif
