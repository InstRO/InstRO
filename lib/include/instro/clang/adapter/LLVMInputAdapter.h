#ifndef INSTRO_CLANG_LLVMINPUTADAPTER_H
#define INSTRO_CLANG_LLVMINPUTADAPTER_H

#include <fstream>

#include "instro/clang/core/ConstructSet.h"
#include "instro/clang/core/ClangAdapterPass.h"

namespace InstRO {
namespace Clang {
class LLVMInputAdapter : public InstRO::Clang::Core::ClangPassImplementation {
 public:
	LLVMInputAdapter(InstRO::Core::ChannelConfiguration cfg);

	bool VisitFunctionDecl(clang::FunctionDecl *fDecl) override;

	void init() override;
	void execute() override;
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
