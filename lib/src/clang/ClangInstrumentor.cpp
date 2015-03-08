#include "instro/clang/ClangInstrumentor.h"

#if 0
InstRO::Clang::ClangInstrumentor::ClangInstrumentor(
		int argc, const char** argv,
		::InstRO::Core::PassManagement::PassManager* manager)
		: argc(argc), argv(argv), fac(new ::InstRO::Clang::PassFactory(manager)) {}
#endif

InstRO::Clang::ClangInstrumentor::ClangInstrumentor(int argc, const char** argv,
																										void* llvmThing)
		: argc(argc),
			argv(argv),
			bla(llvmThing),
			cop(argc, argv, *static_cast<llvm::cl::OptionCategory*>(bla)),
			tool(cop.getCompilations(), cop.getSourcePathList()) {}

InstRO::Core::PassFactory* ::InstRO::Clang::ClangInstrumentor::getFactory(
		CompilationPhase phase) {
	if (fac == nullptr) {
		std::unique_ptr<InstRO::Clang::PassFactory> t(
				new InstRO::Clang::PassFactory(getPassManager()));
		fac = std::move(t);
	}
	return fac.get();
}

void InstRO::Clang::ClangInstrumentor::init() {}

// extern llvm::cl::OptionCategory MyTool;

void InstRO::Clang::ClangInstrumentor::apply() {
	std::cout << "Preparing to run Clang tool" << std::endl;
	InstRO::Clang::Support::ClangConsumerFactory f(getPassManager(),
																								 tool.getReplacements());
	tool.run(clang::tooling::newFrontendActionFactory<
							 InstRO::Clang::Support::ClangConsumerFactory>(&f).get());
}

void InstRO::Clang::ClangInstrumentor::finalize() {}
