#include <memory>
#include "instro/utility/MemoryManagement.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/tooling/RoseConstructClassInterface.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace ConstructClassInterface {

InstRO::Core::ConstructSet RoseConstructClassInterface::getConstructsByClass(
		const InstRO::Core::ConstructTraitType constructClass) {

	InstRO::Core::ConstructSet result;
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface csci(&result);

	for (auto sgNode : SageInterface::querySubTree<SgNode>(proj, V_SgNode)) {
		if (InstRO::Rose::Core::RoseConstructLevelPredicates::ConstructPredicate()(sgNode)) {
			auto construct = InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(sgNode);
			if (construct->getTraits().is(constructClass)) {
				csci.put(construct);
			}
		}
	}

	return result;
}

}	// ConstructClassInterface
}	// Tooling
}	// Rose
}	// InstRO
