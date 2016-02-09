#ifndef INSTRO_ROSE_TOOLING_ROSE_CONSTRUCT_CLASS_INTERFACE_H
#define INSTRO_ROSE_TOOLING_ROSE_CONSTRUCT_CLASS_INTERFACE_H

#include "instro/core/ConstructSet.h"
#include "instro/tooling/ConstructTraitInterface.h"

#include "rose.h"

namespace InstRO {
namespace Rose {
namespace Tooling {

namespace ConstructTraitInterface {

class RoseConstructTraitInterface : public InstRO::Tooling::ConstructTraitInterface::ConstructTraitInterface {
 protected:
	SgProject *proj;

 public:
	RoseConstructTraitInterface() = delete;
	RoseConstructTraitInterface(SgProject *projArg) : proj(projArg) {}

	InstRO::Core::ConstructSet getConstructsByTrait(const InstRO::Core::ConstructTraitType constructTrait) override;
};
}
}
}
}

#endif
