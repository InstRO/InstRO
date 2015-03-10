#include "instro/clang/core/ClangPassExecuter.h"

InstRO::Clang::Core::PassManagement::ClangPassExecuter::ClangPassExecuter(
		clang::ASTContext *context)
		: context(context) {}

void InstRO::Clang::Core::PassManagement::ClangPassExecuter::execute(
		InstRO::PassImplementation *pass) {
	std::cout << "Executing pass " << pass << " with ClangPassExecuter"
						<< std::endl;
	// We are inside the Clang Pass Executer. So cast it!
	InstRO::Clang::Core::ClangPassImplementation *pImpl =
			reinterpret_cast<InstRO::Clang::Core::ClangPassImplementation *>(pass);
	// Since we are in clang this is the first time we can set the ASTContext
	// dependent attributes which passes need!
	// Clang/LLVM does not use (or allow) RTTI, so we are not allowed to use
	// dynamic_cast here. Thus we need to check at compile time. std::is_base_of
	// is c++11
	//	if (std::is_base_of<InstRO::Clang::Core::ClangAdapterPass,
	//											std::remove_pointer<decltype(pImpl)>::type>::value) {
	std::cout
			<< "This is a clang pass implementation, we are setting the ASContext"
			<< std::endl;
	// Since we are inside a compile-time-type-safe wall, we can safely
	// reinterpret cast
	pImpl->setASTContext(context);
	//	}
	// After this is set, we invoke the pass
	std::cout << "RecursiveASTVisitor, so TraverseDecl" << std::endl;
	// Since we are inside a compile-time-type-safe wall, we can safely
	// reinterpret cast
	pImpl->TraverseDecl(context->getTranslationUnitDecl());
}

void InstRO::Clang::Core::PassManagement::ClangPassExecuter::setASTContext(
		clang::ASTContext *context) {
	this->context = context;
}
