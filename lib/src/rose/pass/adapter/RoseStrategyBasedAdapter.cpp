#include <memory>	// Need shared PTR
#include "instro/core/Pass.h"
#include "instro/core/ConstructSet.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/pass/adapter/RoseStrategyBasedAdapter.h"
#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/GenericInstrumentationStrategy.h"

namespace InstRO {
namespace Rose {
namespace Adapter {
std::shared_ptr<InstRO::Rose::Core::RoseConstruct> isRoseConstruct(std::shared_ptr<InstRO::Core::Construct> c) {
	return std::dynamic_pointer_cast<InstRO::Rose::Core::RoseConstruct>(c);
}

void RoseStrategyBasedAdapter::execute() {
	auto rocsci = InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface(getInput(0));
	for (auto construct : rocsci) {
		auto rC = isRoseConstruct(construct);
		strat_iterator i;

		for (i = strategies.begin(); i != strategies.end(); ++i) {
			if (i->second->isImplementedFor(rC->getNode())) {
				i->second->applyInstrumentationFor(rC->getNode());
				InstRO::InfrastructureInterface::ConstructSetCompilerInterface(getCollisionSet()).put(rC);
				InstRO::InfrastructureInterface::ConstructSetCompilerInterface(&outputSet).put(rC);
				// optional: break to avoid multiple strategies on a single node
			}
		}
	}
}

}	// namespace Adapter
}	// namespace Rose
}	// namespace InstRO
