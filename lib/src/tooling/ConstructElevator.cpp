#include <memory>	// Shared_ptr

#include "instro/utility/MemoryManagement.h"

#include "instro/core/ConstructSet.h"
#include "instro/tooling/ConstructElevator.h"
namespace InstRO {
namespace Tooling {
namespace ConstructElevator {

InstRO::Core::ConstructSet ConstructElevator::crop(const InstRO::Core::ConstructSet& inputCS,
																									 InstRO::Core::ConstructTraitType min,
																									 InstRO::Core::ConstructTraitType max) {
	InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface input(&inputCS);

	InstRO::Core::ConstructSet outputCS;
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface output(&outputCS);

	for (auto construct : input) {
		if (construct->getTraits().min() > max || construct->getTraits().max() < min) {
			continue;
		} else {
			output.put(construct);
		}
	}
	return outputCS;
}
InstRO::Core::ConstructSet ConstructElevator::raise(const Core::ConstructSet* input, Core::ConstructTraitType cl) {
	return raise(*input, cl);
}
InstRO::Core::ConstructSet ConstructElevator::lower(const Core::ConstructSet* input, Core::ConstructTraitType cl) {
	return lower(*input, cl);
}
InstRO::Core::ConstructSet ConstructElevator::crop(const InstRO::Core::ConstructSet* inputCS,
																									 InstRO::Core::ConstructTraitType min,
																									 InstRO::Core::ConstructTraitType max) {
	return crop(*inputCS, min, max);
}

}	// namespace ConstructElevator
}	// namespace Tooling
}	// namespace InstRO
