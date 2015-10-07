#ifndef EXTENDED_CALLGRAPH_COST_OPTIMIZATION_STRATEGY_H
#define EXTENDED_CALLGRAPH_COST_OPTIMIZATION_STRATEGY_H

#include <set>
#include <map>

namespace InstRO {
namespace Tooling {
namespace ExtendedCallGraph {

class ExtendedCallGraph;
class ExtendedCallGraphNode;


/**
 * Helper object to assign cost values
 */
struct ExtendedCallgraphCosts {
	unsigned long RECURSION;
	unsigned long LOOP;
	unsigned long CONDITIONAL;
	unsigned long FUNCTION;
	unsigned long FUNCTION_CALL;
	unsigned long SCOPE;
	unsigned long DEFAULT;
}; // struct ExtendedCallgraphCosts

/**
 * Algorithm to balance instrumentation costs against information gain
 * Basic parameters of the algorithm are Path Costs and Successor Costs
 *
 * Path costs are calculated as a nodes cost factor multiplied by
 * the path costs of the most cost-intense predecessor.
 * Root costs are equal to the nodes cost factor
 *
 * Successor costs are calculated as 1 + the sum of successor costs
 * multiplied by the nodes cost factor.
 * Leaf costs are equal to the nodes cost factor.
 *
 * \author Matthias Zoellner
 */
class ExtendedCallgraphCostOptimizationStrategy {
	/**
	 * Internal structure to gather information about an ECG node in the frontier
	 */
	struct NodeRating {
		NodeRating(ExtendedCallGraphNode* n, unsigned long successorCosts, unsigned long pathCosts)
			: successorCosts(successorCosts), pathCosts(pathCosts), node(n) { }
		unsigned long successorCosts;
		unsigned long pathCosts;
		ExtendedCallGraphNode* node;
	};
	typedef std::multimap<double, NodeRating, std::greater<unsigned long> > FrontierType;
	typedef FrontierType::iterator FrontierIteratorType;
public:
	ExtendedCallgraphCostOptimizationStrategy(ExtendedCallGraph* g, ExtendedCallGraphNode* main);

	ExtendedCallgraphCostOptimizationStrategy(ExtendedCallgraphCosts costConfiguration, ExtendedCallGraph* g, ExtendedCallGraphNode* main);

	/**
	 * Execute the optimization strategy.
	 *
	 * Parameters:
	 *
	 * baseline: select every node below this Path Costs value
	 *
	 * succThreshold: unless in baseline, do not select
	 *     nodes with Successor Costs below this value
	 *
	 * costLimit: select additional nodes above baseline, with
	 *     respect to succThreshold, until costs of selection reach
	 *     this value or no more free nodes are available
	 */
	std::set<ExtendedCallGraphNode*> OptimizeSelectionSet(unsigned long baseline, unsigned long succThreshold, unsigned long costLimit);

	unsigned long SuccessorCosts(ExtendedCallGraphNode* n);

	unsigned long SelectedSuccessorCosts(ExtendedCallGraphNode* n, const std::set<ExtendedCallGraphNode*>& selection);

	/**
	 * Path costs from main function to node n
	 * Path costs can be computed exclusively (inclusiveCosts = false) or
	 * inclusive. Inclusive costs add costs for every processed node,
	 * while exclusive costs only multiplies the call frequency heuristic
	 */
	unsigned long PathCosts(ExtendedCallGraphNode* n, bool inclusiveCosts = false);

private:
	unsigned long Factor(ExtendedCallGraphNode* n);

	unsigned long SelectedSuccessorCostsRecursion(ExtendedCallGraphNode* n, std::set<ExtendedCallGraphNode*>& visitorStack, const std::set<ExtendedCallGraphNode*>& selection);

	/** Returns inclusive path costs, sets callAmount (= exclusive path costs) */
	unsigned long PathCostsRecursion(ExtendedCallGraphNode* n, std::set<ExtendedCallGraphNode*>& visitorStack, unsigned long& callAmount);

	void UpdateFrontierNodesHelper(FrontierType& frontier, std::set<ExtendedCallGraphNode*>& frontierNodes, const std::set<ExtendedCallGraphNode*>& selection, const std::set<ExtendedCallGraphNode*>& nodes);

	ExtendedCallGraph* graph;
	ExtendedCallGraphNode* main;
	ExtendedCallgraphCosts costs;
}; // class ExtendedCallgraphCostOptimizationStrategy


} // namespace ExtendedCallGraph
} // namespace Tooling
} // namespace InstRO

#endif // EXTENDED_CALLGRAPH_COST_OPTIMIZATION_STRATEGY_H

