#ifndef INSTRO_TOOLING_EXTENDED_CALLGRAPH
#define INSTRO_TOOLING_EXTENDED_CALLGRAPH

#include "instro/core/ConstructSet.h"
#include "instro/utility/Logger.h"

#include <set>
#include <map>
#include <iostream>
#include <string>

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
	InstRO::Core::ConstructSet getAssociatedConstructSet() {
		return cs;
	}
	enum ECGNodeType getNodeType() {
		return type;
	}

	void setAssociatedConstructSet(InstRO::Core::ConstructSet cs) {
		this->cs = cs;
	}

	std::string toDotString() {
		return cs.toDotString();
	}

	friend std::ostream& operator<<(std::ostream& out, const ExtendedCallGraphNode& node) {
		out << node.cs;
		return out;
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
	virtual ~ExtendedCallGraph();

	virtual std::set<ExtendedCallGraphNode*> getNodeSet(Core::ConstructSet *cs);

	/** predicate is a functional with parameters(ExtendedCallGraphNode*, ExtendedCallGraph*) */
	template <typename T>
	std::set<ExtendedCallGraphNode*> getNodeSet(T& predicate);
	std::set<ExtendedCallGraphNode*> getNodeSet();

	/* Construction XXX maybe move these to a Builder object? */
	ExtendedCallGraphNode* getNodeWithExactConstructSet(InstRO::Core::ConstructSet cs);
	void addEdge(ExtendedCallGraphNode* from, ExtendedCallGraphNode* to);
	ExtendedCallGraphNode* addNode(ExtendedCallGraphNode* node);
	void swapConstructSet(InstRO::Core::ConstructSet oldCS, InstRO::Core::ConstructSet newCS);
	/** redirect edges of pred and succ if true */
	void removeNode(ExtendedCallGraphNode* node, bool redirectEdges);


	std::set<ExtendedCallGraphNode*> getPredecessors(ExtendedCallGraphNode* start);
	std::set<ExtendedCallGraphNode*> getSuccessors(ExtendedCallGraphNode* start);
	int getPredecessorCount(ExtendedCallGraphNode* start);
	int getSuccessorCount(ExtendedCallGraphNode* start);

	InstRO::Core::ConstructSet getAllReachablePredecessors(std::set<ExtendedCallGraphNode*> startNodes);
	InstRO::Core::ConstructSet getAllReachableSuccessors(std::set<ExtendedCallGraphNode*> startNodes);

	void dump();

	void print(std::string filename, std::set<ExtendedCallGraphNode*> specialNodes = std::set<ExtendedCallGraphNode*>());

 private:
	/** no copying allowed */
	ExtendedCallGraph(const ExtendedCallGraph&) {}
	ExtendedCallGraph& operator =(const ExtendedCallGraph& o) {
		return *this;
	}

	std::string dumpToDotString(ExtendedCallGraphNode* node, std::string fillcolor = std::string("white"));

 private:
	std::map<ExtendedCallGraphNode*, std::set<ExtendedCallGraphNode*> > predecessors;
	std::map<ExtendedCallGraphNode*, std::set<ExtendedCallGraphNode*> > successors;

	std::map<InstRO::Core::ConstructSet, ExtendedCallGraphNode*> csToGraphNode;

	InstRO::Core::ConstructSet getConstructSet(std::set<ExtendedCallGraphNode*> graphNodes);
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
