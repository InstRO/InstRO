#ifndef INSTRO_CONSTRUCT_ELEVATOR
#define INSTRO_CONSTRUCT_ELEVATOR

#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Tooling {

namespace ConstructElevator {

class ConstructElevator {
 public:
	virtual ~ConstructElevator() {}

	// This is the implicit way that the PassManager will always apply
	virtual Core::ConstructSet raise(const Core::ConstructSet *input, Core::ConstructTraitType cl) = 0;
	virtual Core::ConstructSet raise(const Core::ConstructSet &input, Core::ConstructTraitType cl) = 0;
	// This is an explicit function used in very rare circumstances by e.g. a specialized selection pass (if at all)
	virtual Core::ConstructSet lower(const Core::ConstructSet *input, Core::ConstructTraitType cl);
	virtual Core::ConstructSet lower(const Core::ConstructSet &input, Core::ConstructTraitType cl) = 0;

	// Crop Construct up to, or down to a level. Compiler independent, as it does not require AST access
	virtual Core::ConstructSet crop(const Core::ConstructSet *input, Core::ConstructTraitType min,
																	Core::ConstructTraitType max);
	virtual Core::ConstructSet crop(const Core::ConstructSet &input, Core::ConstructTraitType min,
																	Core::ConstructTraitType max);
};

}
}
}

#endif
