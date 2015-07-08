#include <memory>	// Shared_ptr

#include "instro/core/Helper.h"

#include "instro/core/ConstructSet.h"
#include "instro/tooling/ConstructElevator.h"
namespace InstRO {
namespace Tooling {
namespace ConstructElevator {

std::unique_ptr<InstRO::Core::ConstructSet> ConstructElevator::crop(InstRO::Core::ConstructSet *inputCS,
																																		InstRO::Core::ConstructLevelType min,
																																		InstRO::Core::ConstructLevelType max) {
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface input(inputCS);
	auto outputCS = std::make_unique<InstRO::Core::ConstructSet>();
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface output(outputCS.get());

	for (auto construct : input) {
		if (construct->getLevel() > max || construct->getLevel() < min)
			continue;
		else
			output.put(construct);
	}
	return outputCS;
}
}
}
}
