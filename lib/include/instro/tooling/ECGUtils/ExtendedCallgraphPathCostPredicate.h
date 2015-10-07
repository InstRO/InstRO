#ifndef EXTENDED_CALLGRAPH_PATHCOST_PREDICATE_H
#define EXTENDED_CALLGRAPH_PATHCOST_PREDICATE_H

#include "instro/tooling/ECGUtils/ExtendedCallgraphPredicate.h"


namespace InstRO {
namespace Tooling {
namespace ExtendedCallGraph {

class ExtendedCallgraphCostOptimizationStrategy;


class ExtendedCallgraphPathCostPredicate :
	public InstRO::Tooling::ExtendedCallGraph::ExtendedCallgraphPredicate {
public:
	ExtendedCallgraphPathCostPredicate(
		InstRO::Tooling::ExtendedCallGraph::ExtendedCallgraphCostOptimizationStrategy* strategy,
		unsigned long costLimit);

	bool operator() (
		InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode* n,
		InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph* g) const;

private:
	InstRO::Tooling::ExtendedCallGraph::ExtendedCallgraphCostOptimizationStrategy* strategy;
	unsigned long costLimit;

}; // class ExtendedCallgraphPathCostPredicate

} // namespace ExtendedCallGraph
} // namespace Tooling
} // namespace InstRO

#endif // EXTENDED_CALLGRAPH_PATHCOST_PREDICATE_H

