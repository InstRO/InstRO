#include "instro/llvm/selector/FunctionDefinitionSelector.h"

char InstRO::LLVM::FunctionDefinitionSelector::ID = 0;

InstRO::LLVM::FunctionDefinitionSelector::FunctionDefinitionSelector()
		: FunctionPass(ID), pn("Function Definition Selector Pass") {}

void InstRO::LLVM::FunctionDefinitionSelector::getAnalysisUsage(llvm::AnalysisUsage &info) { info.setPreservesAll(); }

bool InstRO::LLVM::FunctionDefinitionSelector::runOnFunction(llvm::Function &function) {
	std::cout << "Selecting: " << function.getName().str() << std::endl;
	put(&function);
	return false;
}
