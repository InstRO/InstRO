#include "instro/clang/ClangInstrumentor.h"

#if 0
InstRO::Clang::ClangInstrumentor::ClangInstrumentor(
		int argc, const char** argv,
		::InstRO::Core::PassManagement::PassManager* manager)
		: argc(argc), argv(argv), fac(new ::InstRO::Clang::PassFactory(manager)) {}
#endif

InstRO::Clang::ClangInstrumentor::ClangInstrumentor(
		int argc, const char** argv, llvm::cl::OptionCategory& llvmThing)
		: argc(argc),
			argv(argv),
			cop(argc, argv, llvmThing),
			tool(cop.getCompilations(), cop.getSourcePathList()), visitingExecuter(nullptr) {}

InstRO::Core::PassFactory* ::InstRO::Clang::ClangInstrumentor::getFactory(
		CompilationPhase phase) {
	if (fac == nullptr) {
		std::unique_ptr<InstRO::Clang::PassFactory> t(
				new InstRO::Clang::PassFactory(getPassManager(), tool.getReplacements(), &visitingExecuter));
		fac = std::move(t);
	}
	return fac.get();
}

void InstRO::Clang::ClangInstrumentor::init() {}

// extern llvm::cl::OptionCategory MyTool;

void InstRO::Clang::ClangInstrumentor::apply() {
	std::cout << "Preparing to run Clang tool" << std::endl;
	InstRO::Clang::Support::ClangConsumerFactory f(getPassManager(),
																								 tool.getReplacements(), &visitingExecuter);
	tool.runAndSave(clang::tooling::newFrontendActionFactory<
							 InstRO::Clang::Support::ClangConsumerFactory>(&f).get());
}

void InstRO::Clang::ClangInstrumentor::finalize() {}
