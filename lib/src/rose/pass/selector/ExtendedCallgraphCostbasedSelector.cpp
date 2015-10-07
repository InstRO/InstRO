#include "instro/rose/pass/selector/ExtendedCallgraphCostbasedSelector.h"

#include "instro/core/Pass.h"
#include "instro/core/PassImplementation.h"

#include "instro/tooling/ECGUtils/ExtendedCallgraphFilters.h"
#include "instro/tooling/ECGUtils/ExtendedCallgraphCostOptimizationStrategy.h"

#include "instro/rose/tooling/ECGUtils/NondefiningDeclarationPredicate.h"

#include <rose.h>

#include <set>

using InstRO::Core::ChannelConfiguration;
using InstRO::Core::ConstructSet;
using InstRO::PassManagement::Pass;

using InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph;
using InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode;
using InstRO::Tooling::ExtendedCallGraph::NondefiningDeclarationPredicate;
using InstRO::Tooling::ExtendedCallGraph::removeNodesInExtendedCallgraph;
using InstRO::Tooling::ExtendedCallGraph::retainChildNodesInExtendedCallgraph;

using InstRO::Rose::Tooling::ExtendedCallGraph::NondefiningDeclarationPredicate;
using InstRO::Rose::Tooling::ExtendedCallGraph::RoseECGGenerator;

using InstRO::Rose::Selector::ExtendedCallgraphCostbasedSelector;

ExtendedCallgraphCostbasedSelector::ExtendedCallgraphCostbasedSelector(Pass* input)
	: RosePassImplementation(ChannelConfiguration(input)), output(NULL) {
}

void ExtendedCallgraphCostbasedSelector::init() { }

// don't know the order of execution for these functions
void ExtendedCallgraphCostbasedSelector::finalize() {
	delete output;
	output = NULL;
}
// and I don't even know, whether releaseOutput should actually delete the output object
void ExtendedCallgraphCostbasedSelector::releaseOutput() {
	delete output;
	output = NULL;
}

void ExtendedCallgraphCostbasedSelector::execute() {

	SgProject* project = SageInterface::getProject();
	assert(project);

	std::set<SgNode*> whitelist;

	ExtendedCallGraph* graph = RoseECGGenerator().build(project);

	SgFunctionDeclaration* mainFn = SageInterface::findMain(project);

	// TODO how to handle this?
	//GraphNode* mainNode = graph->getGraphNode(mainFn); /* old way, not working with new ECG */
	//ExtendedCallGraphNode* mainNode = graph->getNodeWithExactConstructSet(/* main function node */);
	//ExtendedCallGraphNode* mainNode = graph->getNodeSet(/* main node predicate, expect a set size 1 */);

	// keep only nodes that are connected to main
	// this way, selection of unconnected fragments which are of much greater call depth is avoided

	removeNodesInExtendedCallgraph(graph, NondefiningDeclarationPredicate());

	retainChildNodesInExtendedCallgraph(graph, mainNode);

	// Optimization of selected nodes
	ExtendedCallgraphCostOptimizationStrategy strategy(costConfiguration, graph, mainNode);

	std::set<ExtendedCallGraphNode*> filtered = strategy.OptimizeSelectionSet(baseline, threshold, costLimit);

	for (auto i : filtered) {
		// TODO MZ: not sure what to do here, since the whole ECG is different in InstRO V5
		//whitelist.insert(graph->getSgNode(i)); /* old way */
	}

	delete graph;

	throw "Not implemented";
}



