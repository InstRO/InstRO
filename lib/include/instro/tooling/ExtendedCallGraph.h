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
	~ExtendedCallGraph();

	virtual std::set<ExtendedCallGraphNode*> getNodeSet(Core::ConstructSet *cs);

	void addEdge(ExtendedCallGraphNode* from, ExtendedCallGraphNode* to);
	ExtendedCallGraphNode* addNode(ExtendedCallGraphNode* node);
	void swapConstructSet(InstRO::Core::ConstructSet oldCS, InstRO::Core::ConstructSet newCS);

	/** redirect edges of pred and succ if true */
	void removeNode(ExtendedCallGraphNode* node, bool redirectEdges);


	/** predicate is a functional with parameters(ExtendedCallGraphNode*, ExtendedCallGraph*) */
	template <typename T>
	std::set<ExtendedCallGraphNode*> getNodeSet(T& predicate);
	std::set<ExtendedCallGraphNode*> getNodeSet();


	std::set<ExtendedCallGraphNode*> getPredecessors(ExtendedCallGraphNode* start);
	std::set<ExtendedCallGraphNode*> getSuccessors(ExtendedCallGraphNode* start);
	int getPredecessorCount(ExtendedCallGraphNode* start);
	int getSuccessorCount(ExtendedCallGraphNode* start);

	ExtendedCallGraphNode* getGraphNode(InstRO::Core::ConstructSet cs);

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

	std::map<InstRO::Core::ConstructSet, ExtendedCallGraphNode*> csToGraphNode;
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
