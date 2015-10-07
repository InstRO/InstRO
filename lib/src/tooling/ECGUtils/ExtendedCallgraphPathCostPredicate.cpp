#include "instro/tooling/ECGUtils/ExtendedCallgraphPathCostPredicate.h"

#include "instro/tooling/ECGUtils/ExtendedCallgraphCostOptimizationStrategy.h"

using InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph;
using InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode;

using InstRO::Tooling::ExtendedCallGraph::ExtendedCallgraphPathCostPredicate;
using InstRO::Tooling::ExtendedCallGraph::ExtendedCallgraphCostOptimizationStrategy;

ExtendedCallgraphPathCostPredicate::ExtendedCallgraphPathCostPredicate(
	ExtendedCallgraphCostOptimizationStrategy* strategy, unsigned long costLimit)
	: strategy(strategy), costLimit(costLimit) {

}

bool ExtendedCallgraphPathCostPredicate::operator() (ExtendedCallGraphNode* n, ExtendedCallGraph* g) const {
	return strategy->PathCosts(n, true) <= costLimit;
}

