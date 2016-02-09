#ifndef INSTRO_ROSE_TOOLING_ROSE_CONSTRUCT_CLASS_INTERFACE_H
#define INSTRO_ROSE_TOOLING_ROSE_CONSTRUCT_CLASS_INTERFACE_H

#include "instro/core/ConstructSet.h"
#include "instro/tooling/ConstructClassInterface.h"

#include "rose.h"

namespace InstRO {
namespace Rose {
namespace Tooling {

namespace ConstructClassInterface {

class RoseConstructClassInterface : public InstRO::Tooling::ConstructClassInterface::ConstructClassInterface {
 protected:
	SgProject *proj;

 public:
	RoseConstructClassInterface() = delete;
	RoseConstructClassInterface(SgProject *projArg) : proj(projArg) {}

	InstRO::Core::ConstructSet getConstructsByClass(const InstRO::Core::ConstructTraitType constructClass) override;
};
}
}
}
}

#endif
