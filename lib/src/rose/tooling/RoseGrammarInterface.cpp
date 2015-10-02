#include <memory>
#include "instro/core/Helper.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/tooling/RoseGrammarInterface.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace GrammarInterface {

std::unique_ptr<InstRO::Core::ConstructSet> RoseGrammarInterface::getConstructsByClass(
		const InstRO::Core::ConstructTraitType constructClass) {

	std::unique_ptr<InstRO::Core::ConstructSet> result = std::make_unique<InstRO::Core::ConstructSet>();
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface csci(result.get());

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

}	// GrammarInterface
}	// Tooling
}	// Rose
}	// InstRO
