#include "instro/clang/ClangInstrumentor.h"
#include "instro/utility/MemoryManagement.h"
#include "instro/clang/core/ClangConsumerFactory.h"

#include "instro/core/Singleton.h"
#include "instro/utility/exception.h"

InstRO::Clang::ClangInstrumentor::ClangInstrumentor(int argc, const char** argv, llvm::cl::OptionCategory& llvmThing)
		: argc(argc),
			argv(argv),
			cop(argc, argv, llvmThing, llvm::cl::NumOccurrencesFlag::Optional),
			tool(cop.getCompilations(), cop.getSourcePathList()) {
	InstRO::setInstrumentorInstance(this);
}

InstRO::Clang::ClangPassFactory* InstRO::Clang::ClangInstrumentor::getFactory(CompilationPhase phase) {
	if (fac == nullptr) {
		fac.reset(new InstRO::Clang::ClangPassFactory(passManager, tool.getReplacements()));
	}
	return fac.get();
}

clang::tooling::RefactoringTool& InstRO::Clang::ClangInstrumentor::getTool() { return tool; }

void InstRO::Clang::ClangInstrumentor::apply() {
	std::cout << "Preparing to run Clang tool" << std::endl;

	InstRO::Clang::Support::ClangConsumerFactory f(passManager, tool.getReplacements(), getFactory());
	tool.runAndSave(clang::tooling::newFrontendActionFactory<InstRO::Clang::Support::ClangConsumerFactory>(&f).get());
}

void InstRO::Clang::ClangInstrumentor::initializeAnalysisManager(clang::ASTContext& context) {
	cam = std::make_unique<InstRO::Clang::Tooling::ClangAnalysisManager>(context);
}

InstRO::Tooling::AnalysisManager* InstRO::Clang::ClangInstrumentor::getAnalysisManager() {
	if (!cam) {
		InstRO::raise_exception("AnalysisManager has not yet been initialized");
	}

	return cam.get();
}
