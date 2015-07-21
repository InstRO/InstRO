#include <memory>	// Shared_ptr

#include "instro/core/Helper.h"

#include "instro/core/ConstructSet.h"
#include "instro/tooling/ConstructElevator.h"
namespace InstRO {
namespace Tooling {
namespace ConstructElevator {

InstRO::Core::ConstructSet ConstructElevator::crop(const InstRO::Core::ConstructSet &inputCS,
																																		InstRO::Core::ConstructTraitType min,
																																		InstRO::Core::ConstructTraitType max) {

	InstRO::InfracstructureInterface::ReadOnlyConstructSetCompilerInterface input(&inputCS);
	auto outputCS = std::make_unique<InstRO::Core::ConstructSet>();
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface output(outputCS.get());

	for (auto construct = input.cbegin(); construct != input.cend();construct++) {
		if (construct->get()->getTraits().min > max || construct->get()->getTraits().max() < min) {
			continue;
		} else {
			output.put(*construct);
		}
	}
	return *outputCS;
}

}	// namespace ConstructElevator
}	// namespace Tooling
}	// namespace InstRO
