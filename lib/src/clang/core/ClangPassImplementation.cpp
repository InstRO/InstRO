#include "instro/clang/core/ClangAdapterPass.h"

void InstRO::Clang::Core::ClangPassImplementation::execute(){
	assert(executer != nullptr);
	std::cout << "Executing inside ClangPassImplementation" << std::endl;
	executer->execute(this);
}
