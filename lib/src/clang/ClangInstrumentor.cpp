#include "instro/clang/ClangInstrumentor.h"

InstRO::Clang::ClangInstrumentor::ClangInstrumentor(
		int argc, const char** argv, ::InstRO::Core::PassManagement::PassManager* manager)
		: argc(argc), argv(argv), fac(new ::InstRO::Clang::PassFactory(manager)) {}

InstRO::Clang::ClangInstrumentor::ClangInstrumentor(int argc,
																											const char** argv)
		: argc(argc), argv(argv) {}
/*
clang::tooling::ToolAction* ::InstRO::Clang::ClangInstrumentor::
		getClangAction() {
	// Generates the clang action that is actually run on the Frontend
	return clang::tooling::newFrontendActionFactory<
						 ::InstRO::Clang::ClangConsumerFactory>(&fac).get();
}*/

InstRO::Core::PassFactory* ::InstRO::Clang::ClangInstrumentor::getFactory(CompilationPhase phase) {
	if (fac == nullptr) {
		std::unique_ptr<::InstRO::Clang::PassFactory> t(
				new ::InstRO::Clang::PassFactory(getPassManager()));
		fac = std::move(t);
	}
	return fac.get();
}

void InstRO::Clang::ClangInstrumentor::init() {}

static llvm::cl::OptionCategory MyTool("my tool");

void InstRO::Clang::ClangInstrumentor::apply() {
	clang::tooling::CommonOptionsParser cop(argc, argv, MyTool);
	clang::tooling::RefactoringTool tool(cop.getCompilations(),
																			 cop.getSourcePathList());
	InstRO::Clang::Support::ClangConsumerFactory f(getPassManager(), tool.getReplacements());
	tool.run(clang::tooling::newFrontendActionFactory<
							 ::InstRO::Clang::Support::ClangConsumerFactory>(&f).get());
}

void ::InstRO::Clang::ClangInstrumentor::finalize() {}
