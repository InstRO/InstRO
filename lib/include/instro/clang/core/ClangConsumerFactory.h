#ifndef INSTRO_CLANG_CLANGCONSUMERFACTORY_H
#define INSTRO_CLANG_CLANGCONSUMERFACTORY_H

#include <memory>

#include "clang/AST/ASTConsumer.h"
#include "clang/Tooling/Core/Replacement.h"
#include "clang/Frontend/FrontendAction.h"

#include "instro/clang/support/InstROASTConsumer.h"

namespace InstRO {
namespace Clang {
namespace Support {
class ClangConsumerFactory : public clang::ASTFrontendAction {
 public:
	ClangConsumerFactory() = delete;
	ClangConsumerFactory(const ClangConsumerFactory &other) = delete;
	ClangConsumerFactory(InstRO::PassManagement::PassManager *manager, clang::tooling::Replacements &replacements,
											 InstRO::Clang::PassFactory *fac);
	~ClangConsumerFactory();
	std::unique_ptr<clang::ASTConsumer> newASTConsumer();
	std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &ci, llvm::StringRef strRef);

 private:
	InstRO::PassManagement::PassManager *manager;
	clang::tooling::Replacements &replacements;
	InstRO::Clang::PassFactory *factory;
};
}	// Support
}	// Clang
}	// Instro
#endif
