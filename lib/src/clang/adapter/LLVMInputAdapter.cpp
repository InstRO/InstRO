#include "instro/clang/adapter/LLVMInputAdapter.h"
#include "instro/utility/Logger.h"

InstRO::Clang::LLVMInputAdapter::LLVMInputAdapter(InstRO::Pass *pId)
		: InstRO::Clang::ClangPassImplBase<LLVMInputAdapter>(
					InstRO::Core::ChannelConfiguration(pId), new InstRO::Clang::NonVisitingPassExecuter<LLVMInputAdapter>()),
			decidingSelector(pId),
			outfileName("instro-temp-file") {}

bool InstRO::Clang::LLVMInputAdapter::VisitFunctionDecl(clang::FunctionDecl *fDecl) { return true; }

void InstRO::Clang::LLVMInputAdapter::releaseOutput() {}

void InstRO::Clang::LLVMInputAdapter::exec() {
	if (context == nullptr) {
		logIt(ERROR) << "ASTContext was null" << std::endl;
		;
		exit(-1);
	}
	std::ofstream outStream;
	outStream.open(outfileName.c_str());

	if (outStream) {
		auto inputSet = getInput(decidingSelector);
		if (inputSet == nullptr) {
			logIt(ERROR) << "inputSet was nullptr" << std::endl;
			exit(-1);
		}
		auto cs = reinterpret_cast<InstRO::Clang::ClangConstructSet *>(inputSet);
		InstRO::Clang::print(outStream, cs, context);
	}

	outStream.close();
}

InstRO::Clang::ClangConstructSet *InstRO::Clang::LLVMInputAdapter::getOutput() { return nullptr; }
