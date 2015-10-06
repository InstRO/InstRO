#ifndef INSTRO_CLANG_LLVMINPUTADAPTER_H
#define INSTRO_CLANG_LLVMINPUTADAPTER_H

#include <fstream>

#include "instro/clang/core/ConstructSet.h"
#include "instro/clang/core/ClangPassImplBase.h"

namespace InstRO {
namespace Clang {
class LLVMInputAdapter : public InstRO::Clang::ClangPassImplBase<LLVMInputAdapter> {
 public:
	LLVMInputAdapter(InstRO::Core::ChannelConfiguration cfg);

	bool VisitFunctionDecl(clang::FunctionDecl *fDecl);

	void init() override;
//	void execute() override;
	void finalize() override;
	void releaseOutput() override;
	InstRO::Clang::ClangConstructSet *getOutput() override;

	void exec() override;

 private:
	InstRO::Pass *decidingSelector;
	const std::string outfileName;
};
}
}

#endif
