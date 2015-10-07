#ifndef EXTENDED_CALLGRAPH_PREDICATE_H
#define EXTENDED_CALLGRAPH_PREDICATE_H

namespace InstRO {
namespace Tooling {
namespace ExtendedCallGraph {

// forward declarations
class ExtendedCallGraph;
class ExtendedCallGraphNode;


class ExtendedCallgraphPredicate {
public:
	virtual bool operator() (ExtendedCallGraphNode* n, ExtendedCallGraph* g) const = 0;
}; // class ExtendedCallgraphPredicate

} // namespace ExtendedCallGraph
} // namespace Tooling
} // namespace InstRO

#endif //EXTENDED_CALLGRAPH_PREDICATE_H

