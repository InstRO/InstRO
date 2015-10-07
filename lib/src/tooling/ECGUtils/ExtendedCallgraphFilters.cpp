#include "instro/tooling/ECGUtils/ExtendedCallgraphFilters.h"

#include "instro/tooling/ExtendedCallGraph.h"
#include "instro/tooling/ECGUtils/ExtendedCallgraphPredicate.h"
#include "instro/tooling/ECGUtils/GraphNodeTypePredicate.h"
#include "instro/tooling/ECGUtils/GraphNodeParentRelationPredicate.h"

using InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph;
using InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode;
using InstRO::Tooling::ExtendedCallGraph::ExtendedCallgraphPredicate;
using InstRO::Tooling::ExtendedCallGraph::GraphNodeTypePredicate;
using InstRO::Tooling::ExtendedCallGraph::GraphNodeParentRelationPredicate;
using InstRO::Tooling::ExtendedCallGraph::ECGNodeType; // enum

void InstRO::Tooling::ExtendedCallGraph::removeNodesInExtendedCallgraph(ExtendedCallGraph* g, enum ECGNodeType toRemove) {

	removeNodesInExtendedCallgraph(g, GraphNodeTypePredicate(toRemove));
}

void InstRO::Tooling::ExtendedCallGraph::removeNodesInExtendedCallgraph(ExtendedCallGraph* g, const ExtendedCallgraphPredicate& predicate) {

	std::set<ExtendedCallGraphNode*> nodes = g->getNodeSet(predicate);

	for (auto node : nodes) {
		g->removeNode(node, true);
	}
}


void InstRO::Tooling::ExtendedCallGraph::retainChildNodesInExtendedCallgraph(ExtendedCallGraph* g, ExtendedCallGraphNode* anchorNode) {

	std::set<ExtendedCallGraphNode*> nodes = g->getNodeSet<const GraphNodeParentRelationPredicate>(
		GraphNodeParentRelationPredicate(anchorNode));

	std::set<ExtendedCallGraphNode*> removalBase = g->getNodeSet();

	for (auto i : nodes) {
		removalBase.erase(i);
	}

	for (auto node : removalBase) {
		g->removeNode(node, true);
	}
}

