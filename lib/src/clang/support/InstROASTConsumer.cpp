#include "instro/clang/support/InstROASTConsumer.h"

#include "instro/clang/core/ClangConstruct.h"
#include "instro/core/Singleton.h"
#include "instro/clang/ClangInstrumentor.h"
#include "instro/clang/ClangPassFactory.h"

#include "instro/utility/Logger.h"

InstRO::Clang::Support::InstROASTConsumer::InstROASTConsumer(InstRO::PassManagement::PassManager *passManager,
																														 InstRO::Clang::ClangPassFactory *fac)
		: passManager(passManager), factory(fac) {}

void InstRO::Clang::Support::InstROASTConsumer::HandleTranslationUnit(clang::ASTContext &context) {
	// Is invoked on every TranslationUnit
	// invoke all passes in the correct order per TranslationUnit
	// We delegate to the pass manager. But before we set the executer
	InstRO::Clang::Core::ClangConstruct::setASTContext(context);
	static_cast<InstRO::Clang::ClangInstrumentor*>(InstRO::getInstrumentorInstance())->initializeAnalysisManager(context);
	factory->finishConstruction(&context);
	
	logIt(DEBUG) << "Calling execute in PassManager" << std::endl;
	passManager->execute();
}
