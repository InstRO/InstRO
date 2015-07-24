#ifndef INSTRO_TOOLING_ControlFlowGraphNode
#define INSTRO_TOOLING_ControlFlowGraphNode

#include <vector>

#include "instro/core/ConstructSet.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>
#include <boost/graph/graphviz.hpp>

using namespace boost;

namespace InstRO {
namespace Tooling {
namespace ControlFlowGraph {

enum CFGNodeType { FUNC_ENTRY, FUNC_EXIT, SCOPE_ENTRY, SCOPE_EXIT, STMT, EXPR };

static const char* ACFGNodeTypeNames[] = {"FUNC_ENTRY", "FUNC_EXIT", "SCOPE_ENTRY", "SCOPE_EXIT", "STMT", "EXPR"};

class ControlFlowGraphNode {
 public:
	ControlFlowGraphNode() {}	// TODO necessary because of boost stuff, how to get rid of it?

	ControlFlowGraphNode(InstRO::Core::ConstructSet* inputCS, CFGNodeType nodeType) : cs(inputCS), nodeType(nodeType) {}

	// CI: Explicit RAW Pointer. We do not release control of this CS
	InstRO::Core::ConstructSet* getAssociatedConstructSet() { return cs; }
	CFGNodeType getType() { return nodeType; }

	std::string toDotString() {
		return ACFGNodeTypeNames[nodeType];
	}

 protected:
	InstRO::Core::ConstructSet* cs;
	CFGNodeType nodeType;

	friend std::ostream& operator<<(std::ostream& out, const ControlFlowGraphNode& node) {
		out << ACFGNodeTypeNames[node.nodeType] << *(node.cs);
		return out;
	}
};

typedef labeled_graph<adjacency_list<vecS, vecS, directedS, ControlFlowGraphNode>, InstRO::Core::ConstructSet> Graph;


class BoostCFG {
 public:
	BoostCFG() {}

	void addNode(ControlFlowGraphNode cfgNode) {
		auto cs = *cfgNode.getAssociatedConstructSet();
		graph.add_vertex(cs);
		auto& graphNode = graph[cs];
		graphNode = cfgNode;
	}

	void addEdge(InstRO::Core::ConstructSet from, InstRO::Core::ConstructSet to) { add_edge_by_label(from, to, graph); }

	std::vector<ControlFlowGraphNode> findNodes(const InstRO::Core::ConstructSet& constructSet) {
		std::vector<ControlFlowGraphNode> foundNodes;

		// TODO

		return foundNodes;
	}

	void print(std::string name) {
		///XXX
		std::ofstream outputStream;
		outputStream.open(name);
		write_graphviz(outputStream, graph, NodeWriter(graph));
		outputStream.close();
	}

 public:
	Graph graph;

	struct NodeWriter {
		NodeWriter(Graph graph) : g(graph) {}

		template<typename Descriptor>
		void operator()(std::ostream& out, Descriptor key) const {
			auto node = g.graph()[key];
			out << " [label=\"" << node.toDotString() << "\"]";
		}

		Graph g;
	};

};

class ControlFlowGraph {
 public:
	virtual ~ControlFlowGraph() {}

	// TODO implement all of these
	//	virtual ControlFlowGraphNode getCFGEntryNode(std::string name, bool useFullQualification) = 0;
	//	virtual ControlFlowGraphNode getCFGExitNode (std::string name, bool useFullQualification) = 0;
	//	// helpers for Constru
	//	virtual ControlFlowGraphNode getCFGEntryNode(ControlFlowGraphNode) = 0;
	//	virtual ControlFlowGraphNode getCFGExitNode (ControlFlowGraphNode) = 0;
	//	// This function can only be called from the raw interface of the compiler, as the tooling interface only provides
	//construct sets ...
	//	virtual ControlFlowGraphNode getCFGEntryNode(InstRO::Core::Construct) = 0;
	//	virtual ControlFlowGraphNode getCFGExityNode(InstRO::Core::Construct) = 0;
	//
	//	// Get a set of entry/exit nodes for the functions represented by the cs-nodes.
	//	// If a construct in the CS is File or Global-Class no entries are returned for those respecitve constucts
	//	virtual std::set<ControlFlowGraphNode> getCFGEntrySet(InstRO::Core::ConstructSet cs)=0;
	//	virtual std::set<ControlFlowGraphNode> getCFGExitSet(InstRO::Core::ConstructSet cs)=0;
	//
	//	// Find, if possible, the corresponding CFG nodes. Since the CS is a set of nodes, we return a set of nodes ...
	//	virtual std::set<ControlFlowGraphNode> getCFGNodeSet(InstRO::Core::ConstructSet cs) = 0;
};

class AbstractControlFlowGraph : public ControlFlowGraph {
 public:
	AbstractControlFlowGraph(std::vector<BoostCFG> graphs) : cfgs(graphs) {}

	// TODO implement all of these
	//	virtual ControlFlowGraphNode getCFGEntryNode(std::string name, bool useFullQualification) = 0;
	//	virtual ControlFlowGraphNode getCFGExitNode (std::string name, bool useFullQualification) = 0;
	//	// helpers for Constru
	//	virtual ControlFlowGraphNode getCFGEntryNode(ControlFlowGraphNode) = 0;
	//	virtual ControlFlowGraphNode getCFGExitNode (ControlFlowGraphNode) = 0;
	//	// This function can only be called from the raw interface of the compiler, as the tooling interface only provides
	//construct sets ...
	//	virtual ControlFlowGraphNode getCFGEntryNode(InstRO::Core::Construct) = 0;
	//	virtual ControlFlowGraphNode getCFGExityNode(InstRO::Core::Construct) = 0;
	//
	//	// Get a set of entry/exit nodes for the functions represented by the cs-nodes.
	//	// If a construct in the CS is File or Global-Class no entries are returned for those respecitve constucts
	//	virtual std::set<ControlFlowGraphNode> getCFGEntrySet(InstRO::Core::ConstructSet cs)=0;
	//	virtual std::set<ControlFlowGraphNode> getCFGExitSet(InstRO::Core::ConstructSet cs)=0;
	//
	//	// Find, if possible, the corresponding CFG nodes. Since the CS is a set of nodes, we return a set of nodes ...
	//	virtual std::set<ControlFlowGraphNode> getCFGNodeSet(InstRO::Core::ConstructSet cs) = 0;

 private:
	std::vector<BoostCFG> cfgs;
};
}
}
}

#endif
