#ifndef INSTRO_CLANG_LLVMINPUTADAPTER_H
#define INSTRO_CLANG_LLVMINPUTADAPTER_H

#include <fstream>

#include "instro/clang/core/ClangPassImplBase.h"

namespace InstRO {
namespace Clang {
class LLVMInputAdapter : public InstRO::Clang::ClangPassImplBase<LLVMInputAdapter> {
 public:
	LLVMInputAdapter();

	bool VisitFunctionDecl(clang::FunctionDecl *fDecl);

	void exec() override;

 private:
	const std::string outfileName;

	void print(std::ostream &outStream, const InstRO::Core::ConstructSet *cs, clang::ASTContext *astContext);
};
}
}

#endif
