#include "instro/clang/adapter/LLVMInputAdapter.h"

InstRO::Clang::LLVMInputAdapter::LLVMInputAdapter(InstRO::Core::ChannelConfiguration cfg)
		: InstRO::Clang::Core::ClangPassImplementation(cfg), outfileName("instro-temp-file") {}

bool InstRO::Clang::LLVMInputAdapter::VisitFunctionDecl(clang::FunctionDecl *fDecl) {}

void InstRO::Clang::LLVMInputAdapter::init() {}

void InstRO::Clang::LLVMInputAdapter::finalize() {}

void InstRO::Clang::LLVMInputAdapter::releaseOutput() {}

void InstRO::Clang::LLVMInputAdapter::execute() { executer->execute(this); }

void InstRO::Clang::LLVMInputAdapter::exec() {
	std::cout << "Running exec" << std::endl;
	if (context == nullptr) {
		std::cerr << "Context was nullptr" << std::endl;
		exit(-1);
	}
	std::ofstream outStream;
	outStream.open(outfileName.c_str());

	if (outStream) {
		auto inputSet = getInput(decidingSelector);
		if (inputSet == nullptr) {
			std::cerr << "inputSet was nullptr" << std::endl;
			exit(-1);
		}
		auto cs = reinterpret_cast<InstRO::Clang::ClangConstructSet *>(inputSet);
		InstRO::Clang::print(outStream, cs, context);
	}

	outStream.close();
}

InstRO::Clang::ClangConstructSet *InstRO::Clang::LLVMInputAdapter::getOutput() { return nullptr; }
