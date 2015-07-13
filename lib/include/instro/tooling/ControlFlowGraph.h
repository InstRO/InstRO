#ifndef INSTRO_TOOLING_ControlFlowGraphNode
#define INSTRO_TOOLING_ControlFlowGraphNode
#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface
#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Tooling {
namespace ControlFlowGraph {
class ControlFlowGraphNode {
protected:
	Core::ConstructSet * cs;
public:
	ControlFlowGraphNode(Core::ConstructSet * inputCS):cs(inputCS){}
	// CI: Explicit RAW Pointer. We do not release control of this CS
	virtual Core::ConstructSet *getAssociatedConstructSet() { return cs;};
};
class ControlFlowGraph {
public:
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
