#ifndef INSTRO_CLANG_PASS_ADAPTER_CLANGMANGLEDNAMEOUTPUTADAPTER_H
#define INSTRO_CLANG_PASS_ADAPTER_CLANGMANGLEDNAMEOUTPUTADAPTER_H

#include <fstream>

#include "instro/clang/core/ClangPassImplBase.h"

namespace InstRO {
namespace Clang {
namespace Adapter {

class ClangMangledNameOutputAdapter : public InstRO::Clang::ClangPassImplBase<ClangMangledNameOutputAdapter> {
 public:
	ClangMangledNameOutputAdapter();

	bool VisitFunctionDecl(clang::FunctionDecl* fDecl);

	void exec() override;

 private:
	const std::string outfileName;

	void print(std::ostream& outStream, const InstRO::Core::ConstructSet* cs, clang::ASTContext* astContext);
};
}	// namespace Adapter
}	// namespace Clang
}	// namespace InstRO

#endif
