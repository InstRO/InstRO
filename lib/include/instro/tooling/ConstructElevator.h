#ifndef INSTRO_TOOLING_CONSTRUCT_ELEVATOR_H
#define INSTRO_TOOLING_CONSTRUCT_ELEVATOR_H

#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Tooling {

namespace ConstructElevator {

class ConstructElevator {
 public:
	virtual ~ConstructElevator() {}

	// This is the implicit way that the PassManager will always apply
	Core::ConstructSet raise(const Core::ConstructSet *input, Core::ConstructTraitType cl);
	virtual Core::ConstructSet raise(const Core::ConstructSet &input, Core::ConstructTraitType cl) = 0;

	// This is an explicit function used in very rare circumstances by e.g. a specialized selection pass (if at all)
	Core::ConstructSet lower(const Core::ConstructSet *input, Core::ConstructTraitType cl);
	virtual Core::ConstructSet lower(const Core::ConstructSet &input, Core::ConstructTraitType cl) = 0;

	// Crop Construct up to, or down to a level. Compiler independent, as it does not require AST access
	Core::ConstructSet crop(const Core::ConstructSet *input, Core::ConstructTraitType min,
																	Core::ConstructTraitType max);
	Core::ConstructSet crop(const Core::ConstructSet &input, Core::ConstructTraitType min,
																	Core::ConstructTraitType max);
};

}
}
}

#endif
