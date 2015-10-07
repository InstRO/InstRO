#include "instro/tooling/ECGUtils/ExtendedCallgraphCostOptimizationStrategy.h"

#include "instro/tooling/ExtendedCallGraph.h"
#include "instro/tooling/ECGUtils/ExtendedCallgraphPathCostPredicate.h"

#include <cassert>

using InstRO::Tooling::ExtendedCallGraph::ExtendedCallgraphCostOptimizationStrategy;

using InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph;
using InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode;


ExtendedCallgraphCostOptimizationStrategy::ExtendedCallgraphCostOptimizationStrategy(ExtendedCallGraph* g, ExtendedCallGraphNode* main)
	: graph(g), main(main) {

	costs.RECURSION = 10000;
	costs.LOOP = 1000;
	costs.CONDITIONAL = 1;
	costs.FUNCTION = 1;
	costs.FUNCTION_CALL = 1;
	costs.SCOPE = 1;
	costs.DEFAULT = 1;
}


ExtendedCallgraphCostOptimizationStrategy::ExtendedCallgraphCostOptimizationStrategy(ExtendedCallgraphCosts costConfiguration, ExtendedCallGraph* g, ExtendedCallGraphNode* main)
	: graph(g), main(main), costs(costConfiguration) {

}


std::set<ExtendedCallGraphNode*> ExtendedCallgraphCostOptimizationStrategy::OptimizeSelectionSet(unsigned long baseline, unsigned long succThreshold, unsigned long costLimit) {

	// child nodes of the currently selected result set - candidates to be transferred to the results
	FrontierType frontier;
	std::set<ExtendedCallGraphNode*> frontierNodes;

	std::set<ExtendedCallGraphNode*> result = graph->getNodeSet<const ExtendedCallgraphPathCostPredicate>(ExtendedCallgraphPathCostPredicate(this, baseline));

	// initialize selection costs
	unsigned long totalCosts = SelectedSuccessorCosts(main, result);
	std::cout << "Initial SelectedSuccessorCosts = " << totalCosts << std::endl;

	// initialize frontier
	for (auto i : result) {
		UpdateFrontierNodesHelper(frontier, frontierNodes, result, graph->getSuccessors(i));
	}

	// add nodes to selection until cost limit is reached or all candidate nodes are added
	while (totalCosts < costLimit && !frontier.empty()) {
		// element with biggest SuccessorCosts is applied first
		FrontierIteratorType it = frontier.begin();
		if (it->second.successorCosts >= succThreshold) {
			// trial insert
			result.insert(it->second.node);
			unsigned long newCosts = totalCosts + PathCosts(it->second.node);

			if (newCosts > costLimit) {
				// undo insert
				result.erase(it->second.node);
			}
			else {
				// keep inserted node and update costs
				totalCosts = newCosts;
				// update frontier
				UpdateFrontierNodesHelper(frontier, frontierNodes, result, graph->getSuccessors(it->second.node));
			}
		}
		frontierNodes.erase(it->second.node);
		frontier.erase(it);
	}
	// Added individual path costs need to sum up to the selected successor costs
	assert(SelectedSuccessorCosts(main, result) == totalCosts);

	return result;
}


unsigned long ExtendedCallgraphCostOptimizationStrategy::SuccessorCosts(ExtendedCallGraphNode* n) {
	return SelectedSuccessorCosts(n, graph->getNodeSet());
}


unsigned long ExtendedCallgraphCostOptimizationStrategy::SelectedSuccessorCosts(ExtendedCallGraphNode* n, const std::set<ExtendedCallGraphNode*>& selection) {
	std::set<ExtendedCallGraphNode*> visitorStack;
	return SelectedSuccessorCostsRecursion(n, visitorStack, selection);
}


unsigned long ExtendedCallgraphCostOptimizationStrategy::PathCosts(ExtendedCallGraphNode* n, bool inclusiveCosts) {
	std::set<ExtendedCallGraphNode*> visitorStack;
	unsigned long callAmount = 0;
	unsigned long inclusivePathCosts = PathCostsRecursion(n, visitorStack, callAmount);
	return (inclusiveCosts ? inclusivePathCosts : callAmount);
}


unsigned long ExtendedCallgraphCostOptimizationStrategy::Factor(ExtendedCallGraphNode* n) {
	switch (n->getNodeType()) {
	case DEFAULT: return costs.DEFAULT;
	case FUNCTION: return costs.FUNCTION;
	case FUNCTION_CALL: return costs.FUNCTION_CALL;
	case CONDITIONAL: return costs.CONDITIONAL;
	case LOOP: return costs.LOOP;
	case SCOPE: return costs.SCOPE;

	default: assert(("Unexpected type of ExtendedCallGraphNode!", false));
	}
}


unsigned long ExtendedCallgraphCostOptimizationStrategy::SelectedSuccessorCostsRecursion(ExtendedCallGraphNode* n, std::set<ExtendedCallGraphNode*>& visitorStack, const std::set<ExtendedCallGraphNode*>& selection) {
	// FIX: in a diamond graph, a successor may be selected without its predecessor
	// So recursion cannot stop at the first not selected node
	unsigned long selectionModifier = selection.count(n);
	//if (selection.count(n) == 0) {
	//	return 0;
	//}

	if (visitorStack.count(n) > 0) {
		return selectionModifier * costs.RECURSION;
	}

	std::set<ExtendedCallGraphNode*> succ = graph->getSuccessors(n);
	if (succ.empty()) {
		return selectionModifier * Factor(n);
	}

	unsigned long sum = 0;
	visitorStack.insert(n);
	for (auto i : succ) {
		sum += SelectedSuccessorCostsRecursion(i, visitorStack, selection);
	}
	visitorStack.erase(n);
	return Factor(n) * (selectionModifier + sum);
}


unsigned long ExtendedCallgraphCostOptimizationStrategy::PathCostsRecursion(ExtendedCallGraphNode* n, std::set<ExtendedCallGraphNode*>& visitorStack, unsigned long& callAmount) {
	if (visitorStack.count(n) > 0) {
		callAmount = costs.RECURSION;
		return costs.RECURSION;
	}

	if (n == main) {
		callAmount = 1;
		return Factor(n);
	}

	std::set<ExtendedCallGraphNode*> pred = graph->getPredecessors(n);
	// only main is considered a valid root node - report error
	if (pred.empty()) assert(("Node is not a successor of main!", false));

	callAmount = 0;
	unsigned long sum = 0;
	visitorStack.insert(n);
	for (auto i : pred) {
		unsigned long tmp = 0;
		sum += PathCostsRecursion(i, visitorStack, tmp);
		callAmount += tmp;
	}
	visitorStack.erase(n);
	callAmount *= Factor(n);
	return sum + callAmount;
}


void ExtendedCallgraphCostOptimizationStrategy::UpdateFrontierNodesHelper(FrontierType& frontier, std::set<ExtendedCallGraphNode*>& frontierNodes, const std::set<ExtendedCallGraphNode*>& selection, const std::set<ExtendedCallGraphNode*>& nodes) {
	for (auto n : nodes) {
		if (selection.count(n) == 0 && frontierNodes.count(n) == 0) {
			unsigned long c = SuccessorCosts(n);
			unsigned long p = PathCosts(n);
			// TODO weight information gain vs call amount
			double score = c / p;
			frontier.insert(std::make_pair(score, NodeRating(n, c, p)));
			frontierNodes.insert(n);
		}
	}
}


