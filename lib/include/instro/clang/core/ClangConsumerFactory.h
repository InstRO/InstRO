#ifndef INSTRO_CLANG_CLANGCONSUMERFACTORY_H
#define INSTRO_CLANG_CLANGCONSUMERFACTORY_H

#include <instro.h>
#include <memory>

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Core/Replacement.h"

#include "instro/clang/support/InstROASTConsumer.h"

namespace InstRO {
namespace Clang {
namespace Support {
class ClangConsumerFactory : public clang::ASTFrontendAction {
 public:
	ClangConsumerFactory() = delete;
	ClangConsumerFactory(const ClangConsumerFactory &other) = delete;
	ClangConsumerFactory(InstRO::PassManagement::PassManager *manager, ReplacementsMap& Replacements,
											 InstRO::Clang::ClangPassFactory *fac);
	~ClangConsumerFactory();
	std::unique_ptr<clang::ASTConsumer> newASTConsumer();
	std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &ci, llvm::StringRef strRef);

 private:
	InstRO::PassManagement::PassManager *manager;
	ReplacementsMap &replacements;
	InstRO::Clang::ClangPassFactory *factory;
};
}	// Support
}	// Clang
}	// Instro
#endif
