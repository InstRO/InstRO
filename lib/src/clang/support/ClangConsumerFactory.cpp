#include "instro/clang/core/ClangConsumerFactory.h"

InstRO::Clang::Support::ClangConsumerFactory::ClangConsumerFactory(InstRO::PassManagement::PassManager *manager,
																																	 clang::tooling::Replacements &replacements,
																																	 InstRO::Clang::PassFactory *fac)
		: manager(manager), replacements(replacements), factory(fac){}

InstRO::Clang::Support::ClangConsumerFactory::~ClangConsumerFactory() {}

std::unique_ptr<clang::ASTConsumer> InstRO::Clang::Support::ClangConsumerFactory::newASTConsumer() {
	std::cout << "Creating new ASTConsumer" << std::endl;
	return std::unique_ptr<InstRO::Clang::Support::InstROASTConsumer>(
			new InstRO::Clang::Support::InstROASTConsumer(manager, factory));
}

std::unique_ptr<clang::ASTConsumer> InstRO::Clang::Support::ClangConsumerFactory::CreateASTConsumer(
		clang::CompilerInstance &ci, llvm::StringRef strRef) {
	return std::unique_ptr<clang::ASTConsumer>(new InstRO::Clang::Support::InstROASTConsumer(manager, factory));
}
