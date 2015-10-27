#include "instro/clang/support/InstROASTConsumer.h"

InstRO::Clang::Support::InstROASTConsumer::InstROASTConsumer(InstRO::PassManagement::PassManager *passManager,
																														 InstRO::Clang::PassFactory *fac)
		: passManager(passManager), factory(fac) {}

#if 0
InstRO::Clang::Support::InstROASTConsumer::InstROASTConsumer(
		InstRO::PassManagement::PassManager *passManager, InstRO::Clang::PassManagement::ClangPassExecuter *vExecuter,
		InstRO::Clang::PassManagement::ClangPassExecuter *nvExecuter)
		: passManager(passManager), vExecuter(vExecuter), nvExecuter(nvExecuter) {}
#endif

void InstRO::Clang::Support::InstROASTConsumer::HandleTranslationUnit(clang::ASTContext &context) {
	// Is invoked on every TranslationUnit
	// invoke all passes in the correct order per TranslationUnit
	// We delegate to the pass manager. But before we set the executer
	//	std::cout << "Set Executer object @ " << executer << " from within InstROASTConsumer" << std::endl;
	//	passManager->setExecuter(new InstRO::Clang::Core::PassManagement::ClangPassExecuter(&context));
	factory->finishConstruction(&context);
	std::cout << "Calling execute in PassManager" << std::endl;
	passManager->execute();
}
