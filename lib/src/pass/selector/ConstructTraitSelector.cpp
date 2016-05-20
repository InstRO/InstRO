#include "instro/pass/selector/ConstructTraitSelector.h"

#include "instro/tooling/AnalysisInterface.h"
#include "instro/Instrumentor.h"
#include "instro/core/Singleton.h"

namespace InstRO {
namespace Selector {

void ConstructTraitSelector::execute() {
	InstRO::Tooling::ConstructTraitInterface::ConstructTraitInterface* cti =
			getInstrumentorInstance()->getAnalysisManager()->getConstructTraitInterface();

	outputSet = cti->getConstructsByTrait(constructTrait);
}

}	// namespace Selector
}	// namespace InstRO
