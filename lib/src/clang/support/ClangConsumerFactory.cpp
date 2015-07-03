#include "instro/clang/core/ClangConsumerFactory.h"

InstRO::Clang::Support::ClangConsumerFactory::ClangConsumerFactory(
		InstRO::PassManagement::PassManager *manager, clang::tooling::Replacements &replacements,
		InstRO::Clang::PassManagement::ClangPassExecuter *vExecuter,
		InstRO::Clang::PassManagement::ClangPassExecuter *nvExecuter)
		: manager(manager), vExecuter(vExecuter), nvExecuter(nvExecuter), replacements(replacements) {}

InstRO::Clang::Support::ClangConsumerFactory::~ClangConsumerFactory() {}

std::unique_ptr<clang::ASTConsumer> InstRO::Clang::Support::ClangConsumerFactory::newASTConsumer() {
	std::cout << "Creating new ASTConsumer" << std::endl;
	return std::unique_ptr<InstRO::Clang::Support::InstROASTConsumer>(
			new InstRO::Clang::Support::InstROASTConsumer(manager, vExecuter, nvExecuter));
}

std::unique_ptr<clang::ASTConsumer> InstRO::Clang::Support::ClangConsumerFactory::CreateASTConsumer(
		clang::CompilerInstance &ci, llvm::StringRef strRef) {
	return std::unique_ptr<clang::ASTConsumer>(nullptr);
}
