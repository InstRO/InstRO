#include "instro/pass/selector/ConstructClassSelector.h"

#include "instro/tooling/AnalysisInterface.h"
#include "instro/core/Instrumentor.h"
#include "instro/core/Singleton.h"

namespace InstRO {
namespace Selector {

void ConstructClassSelector::execute() {
	InstRO::Tooling::GrammarInterface::GrammarInterface* gi =
			getInstrumentorInstance()->getAnalysisManager()->getGrammarInterface();

	outputSet = gi->getConstructsByClass(constructClass);
}

}	// namespace Selector
}	// namespace InstRO
