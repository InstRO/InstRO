#ifndef NONDEFINING_DECLARATION_PREDICATE_H
#define NONDEFINING_DECLARATION_PREDICATE_H

#include "instro/tooling/ECGUtils/ExtendedCallgraphPredicate"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace ExtendedCallGraph {

class NondefiningDeclarationPredicate : 
	InstRO::Tooling::ExtendedCallGraph::ExtendedCallgraphPredicate {
public:
	bool operator() (ExtendedCallGraphNode* n, ExtendedCallGraph* g) const;
}; // class NondefiningDeclarationPredicate

} // namespace ExtendedCallGraph
} // namespace Tooling
} // namespace Rose
} // namespace InstRO

#endif //NONDEFINING_DECLARATION_PREDICATE_H

