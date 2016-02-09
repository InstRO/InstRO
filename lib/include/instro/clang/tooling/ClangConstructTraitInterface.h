#ifndef INSTRO_CLANG_TOOLING_CLANG_CONSTRUCT_CLASS_INTERFACE_H
#define INSTRO_CLANG_TOOLING_CLANG_CONSTRUCT_CLASS_INTERFACE_H

#include "instro/tooling/ConstructTraitInterface.h"
#include "instro/core/ConstructSet.h"

#include "clang/AST/ASTContext.h"

namespace InstRO {
namespace Clang {
namespace Tooling {
namespace ConstructTraitInterface {

class ClangConstructTraitInterface : public InstRO::Tooling::ConstructTraitInterface::ConstructTraitInterface {

public:
	ClangConstructTraitInterface(clang::ASTContext &context) : context(context) {}
	ClangConstructTraitInterface() = delete;
	virtual ~ClangConstructTraitInterface() {}

	InstRO::Core::ConstructSet getConstructsByTrait(const InstRO::Core::ConstructTraitType constructTrait) override;

protected:
	clang::ASTContext &context;

};

}
}
}
}


#endif // INSTRO_CLANG_TOOLING_CLANG_CONSTRUCT_CLASS_INTERFACE_H
