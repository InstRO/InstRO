#ifndef GRAPH_NODE_TYPE_PREDICATE_H
#define GRAPH_NODE_TYPE_PREDICATE_H

// for the enum that can't be forward declared
#include "instro/tooling/ExtendedCallGraph.h"

#include "instro/tooling/ECGUtils/ExtendedCallgraphPredicate.h"

namespace InstRO {
namespace Tooling {
namespace ExtendedCallGraph {

// forward declarations
class ExtendedCallGraph;
class ExtendedCallGraphNode;

class GraphNodeTypePredicate :
	public InstRO::Tooling::ExtendedCallGraph::ExtendedCallgraphPredicate {
public:
	GraphNodeTypePredicate(enum InstRO::Tooling::ExtendedCallGraph::ECGNodeType type);

	bool operator() (
		InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode* n,
		InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph* g) const;

private:
	enum InstRO::Tooling::ExtendedCallGraph::ECGNodeType type;

}; // class GraphNodeTypePredicate

} // namespace ExtendedCallGraph
} // namespace Tooling
} // namespace InstRO

#endif // GRAPH_NODE_TYPE_PREDICATE_H
