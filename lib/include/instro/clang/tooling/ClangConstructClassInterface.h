#ifndef INSTRO_CLANG_TOOLING_CLANG_CONSTRUCT_CLASS_INTERFACE_H
#define INSTRO_CLANG_TOOLING_CLANG_CONSTRUCT_CLASS_INTERFACE_H

#include "instro/tooling/ConstructClassInterface.h"
#include "instro/core/ConstructSet.h"

#include "clang/AST/ASTContext.h"

namespace InstRO {
namespace Clang {
namespace Tooling {
namespace ConstructClassInterface {

class ClangConstructClassInterface : public InstRO::Tooling::ConstructClassInterface::ConstructClassInterface {

public:
	ClangConstructClassInterface(clang::ASTContext &context) : context(context) {}
	ClangConstructClassInterface() = delete;
	virtual ~ClangConstructClassInterface() {}

	InstRO::Core::ConstructSet getConstructsByClass(const InstRO::Core::ConstructTraitType constructClass) override;

protected:
	clang::ASTContext &context;

};

}
}
}
}


#endif // INSTRO_CLANG_TOOLING_CLANG_CONSTRUCT_CLASS_INTERFACE_H
