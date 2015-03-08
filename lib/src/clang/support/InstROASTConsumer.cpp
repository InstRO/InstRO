#include "instro/clang/support/InstROASTConsumer.h"

::InstRO::Clang::Support::InstROASTConsumer::InstROASTConsumer(
		::InstRO::Core::PassManagement::PassManager *passManager)
		: passManager(passManager) {}

		
void ::InstRO::Clang::Support::InstROASTConsumer::HandleTranslationUnit(
		clang::ASTContext &context) {
	// Is invoked on every TranslationUnit
	// invoke all passes in the correct order per TranslationUnit
	// We delegate to the pass manager. But before we set the executer
	std::cout << "Set Executer from within InstROASTConsumer" << std::endl;
	passManager->setExecuter(new InstRO::Clang::Core::PassManagement::ClangPassExecuter(&context));
	std::cout << "Calling execute in PassManager" << std::endl;
	passManager->execute();
}

