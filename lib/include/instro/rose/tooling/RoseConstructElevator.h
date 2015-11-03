#ifndef INSTRO_ROSE_CONSTRUCT_ELEVATOR
#define INSTRO_ROSE_CONSTRUCT_ELEVATOR

#include "instro/tooling/ConstructElevator.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace ConstructElevator {

class ConstructElevator : public InstRO::Tooling::ConstructElevator::ConstructElevator {
 protected:
 public:
	virtual InstRO::Core::ConstructSet raise(const InstRO::Core::ConstructSet &input,
																					 InstRO::Core::ConstructTraitType cl) override;
	virtual InstRO::Core::ConstructSet raise(const InstRO::Core::ConstructSet *input,
																					 InstRO::Core::ConstructTraitType cl) override;

	virtual InstRO::Core::ConstructSet lower(const InstRO::Core::ConstructSet &input,
																					 InstRO::Core::ConstructTraitType cl) override;
	virtual InstRO::Core::ConstructSet lower(const InstRO::Core::ConstructSet *input,
																					 InstRO::Core::ConstructTraitType cl) override;
};
}
}
}
}
#endif
