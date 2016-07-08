#include "instro/tooling/ExtendedCallGraph.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <queue>

namespace InstRO {
namespace Tooling {
namespace ExtendedCallGraph {


ExtendedCallGraph::ExtendedCallGraph() {
}

ExtendedCallGraph::~ExtendedCallGraph() {
}

ExtendedCallGraphNode* ExtendedCallGraph::addNode(ExtendedCallGraphNode* node) {

	if (csToGraphNode.count(node->getAssociatedConstructSet()) == 0) {

		csToGraphNode[node->getAssociatedConstructSet()] = node;

		predecessors[node] = std::set<ExtendedCallGraphNode*>();
		successors[node] = std::set<ExtendedCallGraphNode*>();
	}
		return csToGraphNode[node->getAssociatedConstructSet()];

}

void ExtendedCallGraph::addEdge(ExtendedCallGraphNode* from, ExtendedCallGraphNode* to) {

	assert(from);
	assert(to);

	auto realFrom = addNode(from);
	auto realTo = addNode(to);

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

std::set<ExtendedCallGraphNode*> ExtendedCallGraph::getNodeSetByCS(const Core::ConstructSet *cs) {
	std::set<ExtendedCallGraphNode*> returnSet;
	for (auto node : getNodeSet()) {
		if (node->getAssociatedConstructSet().intersects(*cs)) {
			returnSet.insert(node);
		}
	}
	return returnSet;
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

InstRO::Core::ConstructSet ExtendedCallGraph::getAllReachablePredecessors(std::set<ExtendedCallGraphNode*> startNodes) {

	std::set<ExtendedCallGraphNode*> visitedNodes = startNodes;
	std::queue<ExtendedCallGraphNode*> todo;
	for (auto n : startNodes) {
		todo.push(n);
	}

	while (!todo.empty()) {
		auto currentNode = todo.front();
		todo.pop();

		for (auto predecessor : getPredecessors(currentNode)) {
			if (visitedNodes.find(predecessor) == visitedNodes.end()) {
				todo.push(predecessor);
				visitedNodes.insert(predecessor);
			}
		}
	}

	return getConstructSet(visitedNodes);
}
InstRO::Core::ConstructSet ExtendedCallGraph::getAllReachableSuccessors(std::set<ExtendedCallGraphNode*> startNodes) {

	std::set<ExtendedCallGraphNode*> visitedNodes = startNodes;
	std::queue<ExtendedCallGraphNode*> todo;
	for (auto n : startNodes) {
		todo.push(n);
	}

	while (!todo.empty()) {
		auto currentNode = todo.front();
		todo.pop();

		for (auto successor : getSuccessors(currentNode)) {
			if (visitedNodes.find(successor) == visitedNodes.end()) {
				todo.push(successor);
				visitedNodes.insert(successor);
			}
		}
	}

	return getConstructSet(visitedNodes);
}

InstRO::Core::ConstructSet ExtendedCallGraph::getConstructSet(std::set<ExtendedCallGraphNode*> graphNodes) {
	InstRO::Core::ConstructSet returnSet;

	for (auto graphNode : graphNodes) {
		returnSet = returnSet.combine(graphNode->getAssociatedConstructSet());
	}

	return returnSet;
}

void ExtendedCallGraph::swapConstructSet(InstRO::Core::ConstructSet oldCS, InstRO::Core::ConstructSet newCS) {

	if (csToGraphNode.find(oldCS) == csToGraphNode.end()) {
		return; // not in graph yet -> nothing to swap
	}

	ExtendedCallGraphNode* graphNode = csToGraphNode[oldCS];
	graphNode->setAssociatedConstructSet(newCS);

	csToGraphNode.erase(oldCS);
	csToGraphNode[newCS] = graphNode;

}

ExtendedCallGraphNode* ExtendedCallGraph::getNodeWithExactConstructSet(InstRO::Core::ConstructSet cs) {
	return csToGraphNode[cs];
}

void ExtendedCallGraph::dump() {

	std::cout << "== Dumping Extended Callgraph ==" << std::endl;

	for (ExtendedCallGraphNode* fromNode : getNodeSet()) {
		std::cout << fromNode->getAssociatedConstructSet() << " : " << getSuccessorCount(fromNode) << std::endl;
		for (ExtendedCallGraphNode* toNode : getSuccessors(fromNode)) {
			std::cout << "  -->\t" << toNode->getAssociatedConstructSet() << std::endl;
		}
	}
}


void ExtendedCallGraph::print(std::string filename, std::set<ExtendedCallGraphNode*> specialNodes) {

	std::ofstream outfile(filename, std::ofstream::out);
	outfile << "digraph callgraph {\nnode [shape=oval, style=filled]\n";

	for (ExtendedCallGraphNode* fromNode : this->getNodeSet()) {

		if (specialNodes.find(fromNode) != specialNodes.end()) {
			outfile << dumpToDotString(fromNode, "yellow") << std::endl;
		} else {
			outfile << dumpToDotString(fromNode) << std::endl;
		}

		for (ExtendedCallGraphNode* toNode : this->getSuccessors(fromNode)) {
			outfile << "\"" << fromNode << "\" -> \"" << toNode << "\"" << std::endl;
		}

	}
	outfile << "\n}" << std::endl;
	outfile.close();
}

std::string ExtendedCallGraph::dumpToDotString(ExtendedCallGraphNode* node, std::string fillcolor) {

	std::string nodeType;
	std::string color = "black";


	switch (node->getNodeType()) {
	case ECGNodeType::FUNCTION:
		nodeType = "FUNCTION";
		color = "red";
		break;
	case ECGNodeType::FUNCTION_CALL:
		nodeType = "CALL";
		color = "green";
		break;
	case ECGNodeType::CONDITIONAL:
		nodeType = "CONDITIONAL";
		color = "blue";
		break;
	case ECGNodeType::LOOP:
		nodeType = "LOOP";
		color = "orange";
		break;
	case ECGNodeType::SCOPE:
		nodeType = "SCOPE";
		break;
	case ECGNodeType::DEFAULT:
		nodeType = "DEFAULT";
		break;
	default:
		assert(false);
	}

	std::stringstream ss;
	ss <<  "\"" << node << "\" [label=\"" << nodeType << "\\n" << node->toDotString()
			<< "\", color=" << color << " , fillcolor=" << fillcolor << "]";

	return ss.str();
}

}	// ExtendedCallGraph
}	// Tooling
}	// InstRO
