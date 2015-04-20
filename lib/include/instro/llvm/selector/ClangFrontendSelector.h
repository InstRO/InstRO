#ifndef INSTRO_LLVM_CLANGFRONTENDSELECTOR_H
#define INSTRO_LLVM_CLANGFRONTENDSELECTOR_H

#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <string>

#include "instro/llvm/core/ConstructSetPassing.h"

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"

namespace InstRO {
namespace LLVM {
class ClangFrontendSelector : public InstRO::LLVM::Core::ConstructSetPassing, public llvm::FunctionPass {
 public:
	ClangFrontendSelector();
	ClangFrontendSelector(std::string filename);

	static char ID;

	const char *getPassName() const { return pn.c_str(); }

	bool runOnFunction(llvm::Function &f) override;

	void parseInputFile();
	std::vector<std::string> explode(std::string s);

 private:
	std::string pn;
	std::string filename;
	std::set<std::string> funcsToMark;
};
}
}

#endif
