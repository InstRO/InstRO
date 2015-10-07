#ifndef GRAPH_NODE_PARENT_RELATION_PREDICATE_H
#define GRAPH_NODE_PARENT_RELATION_PREDICATE_H

namespace InstRO {
namespace Tooling {
namespace ExtendedCallGraph {

class ExtendedCallGraph;
class ExtendedCallGraphNode;

class GraphNodeParentRelationPredicate :
	public InstRO::Tooling::ExtendedCallGraph::ExtendedCallgraphPredicate {
public:
	GraphNodeParentRelationPredicate(InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode* anchorNode);

	bool operator() (
		InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode* n,
		InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph* g) const;

private:
	InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode* anchorNode;

}; // class GraphNodeParentRelationPredicate

} // namespace ExtendedCallGraph
} // namespace Tooling
} // namespace InstRO

#endif // GRAPH_NODE_PARENT_RELATION_PREDICATE_H

