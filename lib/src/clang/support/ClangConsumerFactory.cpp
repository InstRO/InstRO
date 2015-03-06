#include "instro/clang/core/ClangConsumerFactory.h"

InstRO::Clang::Support::ClangConsumerFactory::ClangConsumerFactory(
		::InstRO::Core::PassManagement::PassManager *manager,
		clang::tooling::Replacements &replacements)
		: manager(manager), replacements(replacements) {}

InstRO::Clang::Support::ClangConsumerFactory::~ClangConsumerFactory() {}

std::unique_ptr<clang::ASTConsumer>
InstRO::Clang::Support::ClangConsumerFactory::newASTConsumer() {
	return std::unique_ptr<InstRO::Clang::Support::InstROASTConsumer>(
			new InstRO::Clang::Support::InstROASTConsumer(manager));
}

std::unique_ptr<clang::ASTConsumer> InstRO::Clang::Support::ClangConsumerFactory::CreateASTConsumer(
			clang::CompilerInstance &ci, llvm::StringRef strRef){
	return std::unique_ptr<clang::ASTConsumer>(nullptr);
}


