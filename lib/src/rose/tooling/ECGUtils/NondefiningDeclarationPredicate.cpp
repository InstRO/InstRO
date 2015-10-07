#include "instro/rose/tooling/ECGUtils/NondefiningDeclarationPredicate.h"

using InstRO::Rose::Tooling::ExtendedCallGraph::NondefiningDeclarationPredicate;
using InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph;
using InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode;

bool NondefiningDeclarationPredicate::operator() (ExtendedCallGraphNode* n, ExtendedCallGraph* g) {
	// TODO MZ: how to extract the information, whether the node is a non-defining function declaration?

	/*
	// old code similar to:
	RoseExtendedCallgraph* graph = (RoseExtendedCallgraph*)g;
	SgFunctionDeclaration* decl = isSgFunctionDeclaration(graph->getSgNode(n));
	return decl && !decl->get_definition();
	*/

	throw "Not implemented!";
}

