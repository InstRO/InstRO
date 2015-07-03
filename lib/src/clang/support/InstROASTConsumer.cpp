#include "instro/clang/support/InstROASTConsumer.h"

InstRO::Clang::Support::InstROASTConsumer::InstROASTConsumer(::InstRO::PassManagement::PassManager *passManager)
		: passManager(passManager) {}

InstRO::Clang::Support::InstROASTConsumer::InstROASTConsumer(
		InstRO::PassManagement::PassManager *passManager, InstRO::Clang::PassManagement::ClangPassExecuter *vExecuter,
		InstRO::Clang::PassManagement::ClangPassExecuter *nvExecuter)
		: passManager(passManager), vExecuter(vExecuter), nvExecuter(nvExecuter) {}

void InstRO::Clang::Support::InstROASTConsumer::HandleTranslationUnit(clang::ASTContext &context) {
	// Is invoked on every TranslationUnit
	// invoke all passes in the correct order per TranslationUnit
	// We delegate to the pass manager. But before we set the executer
	//	std::cout << "Set Executer object @ " << executer << " from within InstROASTConsumer" << std::endl;
	//	passManager->setExecuter(new InstRO::Clang::Core::PassManagement::ClangPassExecuter(&context));
	vExecuter->setASTContext(&context);
	nvExecuter->setASTContext(&context);
	std::cout << "Calling execute in PassManager" << std::endl;
	passManager->execute();
}
