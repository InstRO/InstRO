
#include "instro/tooling/ControlFlowGraph.h"

#include "instro/core/Singleton.h"

namespace InstRO {
namespace Tooling {
namespace ControlFlowGraph {

std::set<ControlFlowGraphNode> AbstractControlFlowGraph::getCFGExitSet(InstRO::Core::ConstructSet cs) {

	auto elevator = getInstrumentorInstance()->getAnalysisManager()->getCSElevator();
	InstRO::Core::ConstructSet functions = elevator->raise(cs, InstRO::Core::ConstructTraitType::CTFunction);

	std::set<ControlFlowGraphNode> returnSet;
	for (auto& cfg : cfgs) {
		if (cfg.getStartNode().getAssociatedConstructSet()->intersects(functions)) {
			returnSet.insert(cfg.getStartNode());
		}
	}
	return returnSet;
}

std::set<ControlFlowGraphNode> AbstractControlFlowGraph::getCFGEntrySet(InstRO::Core::ConstructSet cs) {

	auto elevator = getInstrumentorInstance()->getAnalysisManager()->getCSElevator();
	InstRO::Core::ConstructSet functions = elevator->raise(cs, InstRO::Core::ConstructTraitType::CTFunction);

	std::set<ControlFlowGraphNode> returnSet;
	for (auto& cfg : cfgs) {
		if (cfg.getEndNode().getAssociatedConstructSet()->intersects(functions)) {
			returnSet.insert(cfg.getEndNode());
		}
	}
	return returnSet;
}

}	// ControlFlowGraph
}	// Tooling
}	// InstRO
