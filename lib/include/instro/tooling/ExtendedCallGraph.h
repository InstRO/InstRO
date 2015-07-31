#ifndef INSTRO_TOOLING_EXTENDED_CALLGRAPH
#define INSTRO_TOOLING_EXTENDED_CALLGRAPH

#include "instro/core/ConstructSet.h"

#include <set>
#include <map>

namespace InstRO {
namespace Tooling {
namespace ExtendedCallGraph {

enum ECGNodeType {
	DEFAULT,
	FUNCTION,
	FUNCTION_CALL,
	CONDITIONAL,
	LOOP,
	SCOPE
};

class ExtendedCallGraphNode {
 public:
	ExtendedCallGraphNode(InstRO::Core::ConstructSet cs, enum ECGNodeType type) : cs(cs), type(type) {}

	// Explicit RAW Pointer. We do not release control of this CS
	Core::ConstructSet getAssociatedConstructSet() {
		return cs;
	}
	enum ECGNodeType getNodeType() {
		return type;
	}

 private:
	InstRO::Core::ConstructSet cs;
	enum ECGNodeType type;
};

/**
 * \author Roman Ness
 * \author Matthias Zoellner
 * directed edges
 */
class ExtendedCallGraph {
 public:
	ExtendedCallGraph();
	~ExtendedCallGraph();

	// TODO implement
	virtual std::set<ExtendedCallGraphNode*> getECGNodes(Core::ConstructSet *cs) {
		return std::set<ExtendedCallGraphNode*>();
	}

	void addNode(ExtendedCallGraphNode* node);
	void addEdge(ExtendedCallGraphNode* from, ExtendedCallGraphNode* to);

	/** redirect edges of pred and succ if true */
	void removeNode(ExtendedCallGraphNode* node, bool redirectEdges);

	std::set<ExtendedCallGraphNode*> getNodeSet();

	/** predicate is a functional with parameters(ExtendedCallGraphNode*, ExtendedCallGraph*) */
	template <typename T>
	std::set<ExtendedCallGraphNode*> getNodeSet(T& predicate);


	std::set<ExtendedCallGraphNode*> getPredecessors(ExtendedCallGraphNode* start);
	std::set<ExtendedCallGraphNode*> getSuccessors(ExtendedCallGraphNode* start);
	int getPredecessorCount(ExtendedCallGraphNode* start);
	int getSuccessorCount(ExtendedCallGraphNode* start);

 public:
	ExtendedCallGraphNode* addSgNode(InstRO::Core::ConstructSet sgNode, enum ECGNodeType nodeType);
	void swapSgNode(InstRO::Core::ConstructSet oldNode, InstRO::Core::ConstructSet newNode);

	InstRO::Core::ConstructSet getConstructSet(ExtendedCallGraphNode* graphNode);
	ExtendedCallGraphNode* getGraphNode(InstRO::Core::ConstructSet sgNode);

	void dump();

 private:
	/** no copying allowed */
	ExtendedCallGraph(const ExtendedCallGraph&) {}
	ExtendedCallGraph& operator =(const ExtendedCallGraph& o) {
		return *this;
	}

 private:
	std::map<ExtendedCallGraphNode*, std::set<ExtendedCallGraphNode*> > predecessors;
	std::map<ExtendedCallGraphNode*, std::set<ExtendedCallGraphNode*> > successors;

	///TODO rename
	std::map<InstRO::Core::ConstructSet, ExtendedCallGraphNode*> csToGraphNode;
	std::map<ExtendedCallGraphNode*, InstRO::Core::ConstructSet> graphNodeToCs;
};

template <typename T>
std::set<ExtendedCallGraphNode*> ExtendedCallGraph::getNodeSet(T& predicate) {

	std::set<ExtendedCallGraphNode*> results;

	std::map<ExtendedCallGraphNode*, std::set<ExtendedCallGraphNode*> >::iterator it;
	for (it = predecessors.begin(); it != predecessors.end(); ++it) {
		if (predicate(it->first, this)) {
			results.insert(it->first);
		}
	}

	return results;
}

}	// ExtendedCallGraph
}	// Tooling
}	// InstRO

#endif // INSTRO_TOOLING_EXTENDED_CALLGRAPH
