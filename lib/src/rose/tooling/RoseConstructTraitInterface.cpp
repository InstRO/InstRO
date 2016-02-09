#include <memory>
#include "instro/utility/MemoryManagement.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/tooling/RoseConstructTraitInterface.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace ConstructTraitInterface {

InstRO::Core::ConstructSet RoseConstructTraitInterface::getConstructsByTrait(
		const InstRO::Core::ConstructTraitType constructTrait) {

	InstRO::Core::ConstructSet result;
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface csci(&result);

	for (auto sgNode : SageInterface::querySubTree<SgNode>(proj, V_SgNode)) {
		if (InstRO::Rose::Core::RoseConstructLevelPredicates::ConstructPredicate()(sgNode)) {
			auto construct = InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(sgNode);
			if (construct->getTraits().is(constructTrait)) {
				csci.put(construct);
			}
		}
	}

	return result;
}

}	// ConstructTraitInterface
}	// Tooling
}	// Rose
}	// InstRO
