#ifndef EXTENDED_CALLGRAPH_FILTERS_H
#define EXTENDED_CALLGRAPH_FILTERS_H

#include "instro/tooling/ExtendedCallGraph.h"

/**
 * This file contains helper functions to work on an extended callgraph
 */

namespace InstRO {
namespace Tooling {
namespace ExtendedCallGraph {

// forward declarations
class ExtendedCallGraph;
class ExtendedCallGraphNode;
class ExtendedCallgraphPredicate;


/**
 * Removes all nodes of a specific type from the graph, restructuring the affected edges
 */
void removeNodesInExtendedCallgraph(
	InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph* g,
	enum InstRO::Tooling::ExtendedCallGraph::ECGNodeType toRemove);

/**
 * Remove all nodes where the predicate evaluates to true
 */
void removeNodesInExtendedCallgraph(
	InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph* g,
	const InstRO::Tooling::ExtendedCallGraph::ExtendedCallgraphPredicate& predicate);

/**
 * Removes all nodes, which are not anchorNode or in a transitive child relationship to anchorNode
 */
void retainChildNodesInExtendedCallgraph(
	InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph* g,
	InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode* anchorNode);

} // namespace ExtendedCallGraph
} // namespace Tooling
} // namespace InstRO

#endif // EXTENDED_CALLGRAPH_FILTERS_H

