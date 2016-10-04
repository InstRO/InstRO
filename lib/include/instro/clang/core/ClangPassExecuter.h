#ifndef INSTRO_CLANG_CORE_PASSMANAGEMENT_PASSEXECUTOR_H
#define INSTRO_CLANG_CORE_PASSMANAGEMENT_PASSEXECUTOR_H

#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"

#include "instro/core/PassImplementation.h"

#include <iostream>
#include <cassert>

namespace InstRO {
namespace Clang {

template <typename T>
class ClangPassImplBase;

/**
 * The interface serves as delegate to be able to decide whether a traversal is
 * necessary or not. Further it is needed to transport the ASTContext
 * information all the way down to the ClangPassImplBase specialization. We use
 * the same trick that Clang uses by templatizing this with the most derived
 * type to static_cast it for invocation. With that we get a nice and clean
 * interface but still inherit from RecursiveASTVisitor at a fairly high stage.
 */
template <typename T>
class PassExecuter {
 public:
	PassExecuter(){};
	PassExecuter(clang::ASTContext *ctx) : context(ctx){};
	~PassExecuter() {}
	virtual void execute(T *pass) = 0;

	virtual void setASTContext(clang::ASTContext *ctxt) { context = ctxt; };

 protected:
	clang::ASTContext *context;
};

/*
 * Visitor implementation of the executer interface.
 * This implementation invokes the TraverseDecl(TranslationUnitDecl) method on
 * the pass object.
 */
template <typename T>
class VisitingPassExecuter : public PassExecuter<T> {
 public:
	VisitingPassExecuter() : counter(0){};
	VisitingPassExecuter(clang::ASTContext *context);
	void execute(T *pass) override;

 private:
	int counter;
};

/**
 * Non-visitor implementation of the executer interface.
 * This implementation invokes the exec() method (defined in ClangPassImplBase
 * interface) on the pass object.
 */
template <typename T>
class NonVisitingPassExecuter : public PassExecuter<T> {
 public:
	NonVisitingPassExecuter(){};
	NonVisitingPassExecuter(clang::ASTContext *context);
	void execute(T *pass) override;
};

}	// instro
}	// clang

template <typename T>
InstRO::Clang::VisitingPassExecuter<T>::VisitingPassExecuter(clang::ASTContext *context)
		: PassExecuter<T>(context) {}

template <typename T>
void InstRO::Clang::VisitingPassExecuter<T>::execute(T *pass) {
	assert(pass != nullptr);
	assert(context != nullptr);
	assert(context->getTranslationUnitDecl() != nullptr && "translation unit null");
	// traverse the Clang AST
	pass->TraverseDecl(this->context->getTranslationUnitDecl());
}

template <typename T>
InstRO::Clang::NonVisitingPassExecuter<T>::NonVisitingPassExecuter(clang::ASTContext *context)
		: PassExecuter<T>(context) {}

template <typename T>
void InstRO::Clang::NonVisitingPassExecuter<T>::execute(T *pass) {
	assert(pass != nullptr);
	pass->exec();
}
#endif
