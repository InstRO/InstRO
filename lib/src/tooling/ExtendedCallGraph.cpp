#include "instro/tooling/ExtendedCallGraph.h"

#include <cassert>

namespace InstRO {
namespace Tooling {
namespace ExtendedCallGraph {


ExtendedCallGraph::ExtendedCallGraph() {
	// TODO
}

ExtendedCallGraph::~ExtendedCallGraph() {
	// TODO
}

void ExtendedCallGraph::addNode(ExtendedCallGraphNode* node) {
	predecessors[node] = std::set<ExtendedCallGraphNode*>();
	successors[node] = std::set<ExtendedCallGraphNode*>();
}

void ExtendedCallGraph::addEdge(ExtendedCallGraphNode* from, ExtendedCallGraphNode* to) {

	assert(from);
	assert(to);

	auto realFrom = addSgNode(from);
	auto realTo = addSgNode(to);

	predecessors[realTo].insert(realFrom);
	successors[realFrom].insert(realTo);
}

void ExtendedCallGraph::removeNode(ExtendedCallGraphNode* node, bool redirectEdges) {

	for (auto succ : getSuccessors(node)) {
		predecessors[succ].erase(node);
	}

	for (auto pred : getPredecessors(node)) {
		successors[pred].erase(node);

		for (auto succ : getSuccessors(node)) {

			if (redirectEdges) {
				predecessors[succ].insert(pred);
				successors[pred].insert(succ);
			}
		}
	}

	predecessors.erase(node);
	successors.erase(node);
}

std::set<ExtendedCallGraphNode*> ExtendedCallGraph::getNodeSet() {
	std::set<ExtendedCallGraphNode*> nodes;
	for (auto pair : predecessors) {
		nodes.insert(pair.first);
	}
	return nodes;
}

std::set<ExtendedCallGraphNode*> ExtendedCallGraph::getPredecessors(ExtendedCallGraphNode* start) {
	return predecessors[start];
}

std::set<ExtendedCallGraphNode*> ExtendedCallGraph::getSuccessors(ExtendedCallGraphNode* start) {
	return successors[start];
}

int ExtendedCallGraph::getPredecessorCount(ExtendedCallGraphNode* start) {
	return predecessors[start].size();
}

int ExtendedCallGraph::getSuccessorCount(ExtendedCallGraphNode* start) {
	return successors[start].size();
}

//// XXX from ROSE EXTENDED CALLGRAPH


ExtendedCallGraphNode* ExtendedCallGraph::addSgNode(ExtendedCallGraphNode* node) {

	if (csToGraphNode.count(node->getAssociatedConstructSet()) == 0) {

	csToGraphNode[node->getAssociatedConstructSet()] = node;
	graphNodeToCs[node] = node->getAssociatedConstructSet();
	addNode(node);
	}
		return csToGraphNode[node->getAssociatedConstructSet()];

}

void ExtendedCallGraph::swapSgNode(InstRO::Core::ConstructSet oldNode, InstRO::Core::ConstructSet newNode) {

	if (csToGraphNode.find(oldNode) == csToGraphNode.end()) {
		return; // not in graph yet -> nothing to swap
	}

	ExtendedCallGraphNode* graphNode = csToGraphNode[oldNode];

	graphNodeToCs[graphNode] = newNode;

	csToGraphNode.erase(oldNode);
	csToGraphNode[newNode] = graphNode;

}

InstRO::Core::ConstructSet ExtendedCallGraph::getConstructSet(ExtendedCallGraphNode* graphNode) {
	return graphNodeToCs[graphNode];
}

ExtendedCallGraphNode* ExtendedCallGraph::getGraphNode(InstRO::Core::ConstructSet sgNode) {
	return csToGraphNode[sgNode];
}

void ExtendedCallGraph::dump() {

	std::cout << "== Dumping Extended Callgraph ==" << std::endl;

	for (ExtendedCallGraphNode* fromNode : getNodeSet()) {
		std::cout << graphNodeToCs[fromNode] << " : " << getSuccessorCount(fromNode) << std::endl;
		for (ExtendedCallGraphNode* toNode : getSuccessors(fromNode)) {
			std::cout << "  -->\t" << graphNodeToCs[toNode] << std::endl;
		}
	}
}

}	// ExtendedCallGraph
}	// Tooling
}	// InstRO
