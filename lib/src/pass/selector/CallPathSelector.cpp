#include "instro/utility/Logger.h"

#include "instro/pass/selector/CallPathSelector.h"

#include "instro/core/Instrumentor.h"
#include "instro/core/Singleton.h"
#include "instro/tooling/AnalysisInterface.h"

#include <set>

namespace InstRO {
namespace Selector {

void CallPathSelector::execute() {

	Tooling::ExtendedCallGraph::ExtendedCallGraph *ecg = getInstrumentorInstance()->getAnalysisManager()->getECG();
	auto fromNodes = ecg->getNodeSetByCS(getInput(0));
	auto toNodes = ecg->getNodeSetByCS(getInput(1));

	logIt(DEBUG) << "ECG - fromNodes:" << fromNodes.size() << " and toNodes: " << toNodes.size() << std::endl;

	auto predecessorsOfTo = ecg->getAllReachablePredecessors(toNodes);

	logIt(DEBUG) << "predecessorsOfTo elements: " << predecessorsOfTo.size() << std::endl;

	auto successorsOfFrom = ecg->getAllReachableSuccessors(fromNodes);

	logIt(DEBUG) << "successorsOfFrom elements: " << successorsOfFrom.size() << std::endl;

	outputSet = predecessorsOfTo.intersect(successorsOfFrom);

	if (!dotName.empty()) {
		ecg->print(dotName, ecg->getNodeSetByCS(&outputSet));
	}
}

}	// Selector
}	// InstRO
