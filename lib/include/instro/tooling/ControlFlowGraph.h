#ifndef INSTRO_TOOLING_ControlFlowGraphNode
#define INSTRO_TOOLING_ControlFlowGraphNode
#include <memory>	// We need shared pointers
#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Tooling {
namespace ControlFlowGraph {

enum CFGNodeType {
	FUNC_ENTRY,
	FUNC_EXIT,
	SCOPE_ENTRY,
	SCOPE_EXIT,
	STMT,
	EXPR
};

static const char* ACFGNodeTypeNames[] = {
		"FUNC_ENTRY",
		"FUNC_EXIT",
		"SCOPE_ENTRY",
		"SCOPE_EXIT",
		"STMT",
		"EXPR"
};

class ControlFlowGraphNode {
public:
	ControlFlowGraphNode(InstRO::Core::ConstructSet * inputCS, CFGNodeType nodeType) :
			cs(inputCS), nodeType(nodeType) {
	}

	// CI: Explicit RAW Pointer. We do not release control of this CS
	InstRO::Core::ConstructSet *getAssociatedConstructSet() {
		return cs;
	}

	protected:
	InstRO::Core::ConstructSet * cs;
	CFGNodeType nodeType;

	friend std::ostream& operator<<(std::ostream& out, const ControlFlowGraphNode& node) {
		out << ACFGNodeTypeNames[node.nodeType] << node.cs;
		return out;
	}
};




class ControlFlowGraph {
public:
	ControlFlowGraph() {}
	// Get Entry (FUNCTION NODE)
	// Get Exit (Function Node)

	virtual InstRO::Tooling::ControlFlowGraph::ControlFlowGraphNode getCFGEntryNode(std::string name, bool useFullQualification) = 0;
	virtual InstRO::Tooling::ControlFlowGraph::ControlFlowGraphNode getCFGExitNode (std::string name, bool useFullQualification) = 0;
	// helpers for Constru
	virtual InstRO::Tooling::ControlFlowGraph::ControlFlowGraphNode getCFGEntryNode(InstRO::Tooling::ControlFlowGraph::ControlFlowGraphNode) = 0;
	virtual InstRO::Tooling::ControlFlowGraph::ControlFlowGraphNode getCFGExitNode (InstRO::Tooling::ControlFlowGraph::ControlFlowGraphNode) = 0;
	// This function can only be called from the raw interface of the compiler, as the tooling interface only provides construct sets ...
	virtual InstRO::Tooling::ControlFlowGraph::ControlFlowGraphNode getCFGEntryNode(InstRO::Core::Construct) = 0;
	virtual InstRO::Tooling::ControlFlowGraph::ControlFlowGraphNode getCFGExityNode(InstRO::Core::Construct) = 0;

	// Get a set of entry/exit nodes for the functions represented by the cs-nodes.
	// If a construct in the CS is File or Global-Class no entries are returned for those respecitve constucts
	virtual std::set<InstRO::Tooling::ControlFlowGraph::ControlFlowGraphNode> getCFGEntrySet(InstRO::Core::ConstructSet cs)=0;
	virtual std::set<InstRO::Tooling::ControlFlowGraph::ControlFlowGraphNode> getCFGExitSet(InstRO::Core::ConstructSet cs)=0;

	// Find, if possible, the corresponding CFG nodes. Since the CS is a set of nodes, we return a set of nodes ...
	virtual std::set<InstRO::Tooling::ControlFlowGraph::ControlFlowGraphNode> getCFGNodeSet(InstRO::Core::ConstructSet cs) = 0;
};

}
}
}



#endif
