#ifndef INSTRO_TOOLING_CONTROLFLOWGRAPH_H
#define INSTRO_TOOLING_CONTROLFLOWGRAPH_H

#include <vector>

#include "instro/core/ConstructSet.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>
#include <boost/graph/graphviz.hpp>

namespace InstRO {
namespace Tooling {
namespace ControlFlowGraph {

enum CFGNodeType { FUNC_ENTRY, FUNC_EXIT, SCOPE_ENTRY, SCOPE_EXIT, STMT, EXPR, NOT_SET };

static const char* ACFGNodeTypeNames[] = {"FUNC_ENTRY", "FUNC_EXIT", "SCOPE_ENTRY", "SCOPE_EXIT",
																					"STMT",				"EXPR",			 "NOT_SET"};

class ControlFlowGraphNode {
 public:
	ControlFlowGraphNode() {}	// TODO necessary because of boost stuff, how to get rid of it?

	ControlFlowGraphNode(InstRO::Core::ConstructSet* inputCS, CFGNodeType nodeType) : cs(inputCS), nodeType(nodeType) {}

	// CI: Explicit RAW Pointer. We do not release control of this CS
	InstRO::Core::ConstructSet* getAssociatedConstructSet() { return cs; }
	CFGNodeType getType() { return nodeType; }

	std::string toDotString() {
		std::string dotString(ACFGNodeTypeNames[nodeType]);
		dotString += "\\n";
		dotString += cs->toDotString();
		return dotString;
	}

 private:
	InstRO::Core::ConstructSet* cs;
	CFGNodeType nodeType;

	friend bool operator<(const ControlFlowGraphNode& node1, const ControlFlowGraphNode& node2) {
		return node1.cs < node2.cs;
	}

	friend std::ostream& operator<<(std::ostream& out, const ControlFlowGraphNode& node) {
		out << ACFGNodeTypeNames[node.nodeType] << *(node.cs);
		return out;
	}
};

typedef boost::labeled_graph<boost::adjacency_list<boost::setS, boost::vecS, boost::directedS, ControlFlowGraphNode>,
														 InstRO::Core::ConstructSet> Graph;

class BoostCFG {
 public:
	BoostCFG() {}

	const Graph& getGraph() const { return graph; }

	bool contains(ControlFlowGraphNode cfgNode) {
		auto graphNode = boost::vertex_by_label(*cfgNode.getAssociatedConstructSet(), graph);
		return graphNode != boost::graph_traits<Graph>::null_vertex();
	}

	void setStartNode(ControlFlowGraphNode cfgNode) { startNode = cfgNode; }

	ControlFlowGraphNode getStartNode() { return startNode; }

	void setEndNode(ControlFlowGraphNode cfgNode) { endNode = cfgNode; }

	ControlFlowGraphNode getEndNode() { return endNode; }

	void addNode(ControlFlowGraphNode cfgNode) {
		auto cs = *cfgNode.getAssociatedConstructSet();
		graph.add_vertex(cs);
		auto& graphNode = graph[cs];
		graphNode = cfgNode;
	}

	void addEdge(InstRO::Core::ConstructSet from, InstRO::Core::ConstructSet to) { add_edge_by_label(from, to, graph); }

	void print(std::string name) const {
		std::ofstream outputStream;
		outputStream.open(name);
		boost::write_graphviz(outputStream, graph, NodeWriter(graph));
		outputStream.close();
	}

 private:
	Graph graph;
	ControlFlowGraphNode startNode, endNode;

	struct NodeWriter {
		NodeWriter(Graph graph) : g(graph) {}

		template <typename Descriptor>
		void operator()(std::ostream& out, Descriptor nodeDescriptor) const {
			auto node = g.graph()[nodeDescriptor];
			out << " [label=\"" << node.toDotString() << "\"]";
		}

		Graph g;
	};
};

class ControlFlowGraph {
 public:
	virtual ~ControlFlowGraph() {}

	virtual ControlFlowGraphNode getCFGEntryNode(ControlFlowGraphNode) = 0;
	virtual ControlFlowGraphNode getCFGExitNode(ControlFlowGraphNode) = 0;

	virtual std::set<ControlFlowGraphNode> getCFGEntrySet(InstRO::Core::ConstructSet cs) = 0;
	virtual std::set<ControlFlowGraphNode> getCFGExitSet(InstRO::Core::ConstructSet cs) = 0;

	// Find, if possible, the corresponding CFG nodes. Since the CS is a set of nodes, we return a set of nodes ...
	virtual std::set<ControlFlowGraphNode> getCFGNodeSet(InstRO::Core::ConstructSet cs) = 0;
};

class AbstractControlFlowGraph : public ControlFlowGraph {
 public:
	AbstractControlFlowGraph(std::vector<BoostCFG> graphs) : cfgs(graphs) {}

	ControlFlowGraphNode getCFGEntryNode(ControlFlowGraphNode cfgNode) override {
		for (auto cfg : cfgs) {
			if (cfg.contains(cfgNode)) {
				return cfg.getStartNode();
			}
		}
		throw std::string("ControlFlowGraph Error: found no corresponding CFG");
	}
	ControlFlowGraphNode getCFGExitNode(ControlFlowGraphNode cfgNode) override {
		for (auto cfg : cfgs) {
			if (cfg.contains(cfgNode)) {
				return cfg.getEndNode();
			}
		}
		throw std::string("ControlFlowGraph Error: found no corresponding CFG");
	}

	std::set<ControlFlowGraphNode> getCFGEntrySet(InstRO::Core::ConstructSet cs) override;
	std::set<ControlFlowGraphNode> getCFGExitSet(InstRO::Core::ConstructSet cs) override;

	std::set<ControlFlowGraphNode> getCFGNodeSet(InstRO::Core::ConstructSet cs) override {
		std::set<ControlFlowGraphNode> returnSet;

		for (auto const& boostCFG : cfgs) {
			Graph::vertex_iterator vertexIter, vertexEnd;
			for (boost::tie(vertexIter, vertexEnd) = vertices(boostCFG.getGraph()); vertexIter != vertexEnd; vertexIter++) {
				ControlFlowGraphNode node = boostCFG.getGraph().graph()[*vertexIter];

				if (node.getAssociatedConstructSet()->intersects(cs)) {
					returnSet.insert(node);
				}
			}
		}
		return returnSet;
	}

 private:
	std::vector<BoostCFG> cfgs;
};

}	// namespace ControlFlowGraph
}	// namespace Tooling
}	// namespace InstRO

#endif
