#ifndef INSTRO_ROSE_CONSTRUCT_ELEVATOR
#define INSTRO_ROSE_CONSTRUCT_ELEVATOR

#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface
#include "instro/core/ConstructSet.h"
#include "instro/tooling/ConstructElevator.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace ConstructElevator {
/* CI: The ConstructSet class is intended to be specialized for each compiler interface. It provides the basic
* mechanisms to specify what construct level are contained. */
class ConstructElevator : public InstRO::Tooling::ConstructElevator::ConstructElevator {
 protected:
 public:
	// This is the implicit way that the PassManager will allways apply
	virtual InstRO::Core::ConstructSet raise(const InstRO::Core::ConstructSet &input,
																														InstRO::Core::ConstructTraitType cl) override;
	// This is an explicit function used in very rare circumstances by e.g. a specialized selection pass (if at all)
	virtual InstRO::Core::ConstructSet lower(const InstRO::Core::ConstructSet &input,
																														InstRO::Core::ConstructTraitType cl) override;
};
}
}
}
}
#endif