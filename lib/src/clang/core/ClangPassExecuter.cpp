#include "instro/clang/core/ClangPassExecuter.h"

InstRO::Clang::Core::PassManagement::ClangPassExecuter::ClangPassExecuter(
		clang::ASTContext *context)
		: context(context) {}

void InstRO::Clang::Core::PassManagement::ClangPassExecuter::execute(
		InstRO::PassImplementation *pass) {
	std::cout << "Executing pass " << pass << " with ClangPassExecuter"
						<< std::endl;
	// We are inside the Clang Pass Executer. So cast it!
	// FIXME while this is somewhat "reasonable" it still is a cast, which I don't
	// like. In general we need to make sure, that this execuually is a
	// ClangPassImplementation
	InstRO::Clang::Core::ClangPassImplementation *pImpl =
			reinterpret_cast<InstRO::Clang::Core::ClangPassImplementation *>(pass);
	// Since we are in clang this is the first time we can set the ASTContext
	// dependent attributes which passes need!
	std::cout << "Clang pass implementation: setting the ASContext" << std::endl;
	pImpl->setASTContext(context);
	// After this is set, we invoke the pass
	// traverse the Clang AST
	pImpl->TraverseDecl(context->getTranslationUnitDecl());
	// invoke the execute function of the pass implementation
	pImpl->execute();
}

void InstRO::Clang::Core::PassManagement::ClangPassExecuter::setASTContext(
		clang::ASTContext *context) {
	this->context = context;
}
