#ifndef INSTRO_CONSTRUCT_ELEVATOR
#define INSTRO_CONSTRUCT_ELEVATOR
#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface
#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Tooling {

namespace ConstructElevator {
/* CI: The ConstructSet class is intended to be specialized for each compiler interface. It provides the basic
* mechanisms to specify what construct level are contained. */
class ConstructElevator {
 public:
	virtual ~ConstructElevator() {}

	// This is the implicit way that the PassManager will allways apply
	virtual std::unique_ptr<Core::ConstructSet> raise(Core::ConstructSet *input, Core::ConstructTraitType cl) = 0;
	// This is an explicit function used in very rare circumstances by e.g. a specialized selection pass (if at all)
	virtual std::unique_ptr<Core::ConstructSet> lower(Core::ConstructSet *input, Core::ConstructTraitType cl) = 0;

	// Crop Construct up to, or down to a level. This can be implemented compiler independend, as it does not require AST
	// access
	virtual std::unique_ptr<Core::ConstructSet> crop(Core::ConstructSet *input, Core::ConstructTraitType min,
																									 Core::ConstructTraitType max);
};
}
}
}

#endif
