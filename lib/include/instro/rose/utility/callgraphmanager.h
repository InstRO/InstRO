#ifndef CALLGRAPHMANAGER_H
#define CALLGRAPHMANAGER_H

#include "../common.h"
#include "asthelper.h"

#include <boost/unordered_map.hpp>

namespace InstRO {

typedef boost::unordered_map<SgFunctionDeclaration*, long long> CallerMap;
typedef boost::unordered_map<SgFunctionDeclaration*, CallerMap> CalleeToCallerMap;

/**
 * \brief This class manages interaction with a call graph
 * \author Roman
 */
class CallGraphManager {
 public:
	CallGraphManager(SgProject* project, CalleeToCallerMap* callsByCaller = NULL);

	/* Get the corresponding SgGraphNode for a Function Declaration */
	SgGraphNode* getCallGraphNode(SgFunctionDeclaration* funcDecl);

	SgFunctionDeclaration* getFunctionDeclaration(SgGraphNode* node);

	/* Return all descendants in the call graph up to a level */
	std::set<SgGraphNode*> getDescendantsTillDepth(SgGraphNode* parent, int remainingDepth);
	/* Return all ancestors in the call graph up to a level */
	std::set<SgGraphNode*> getAncestorsTillDepth(SgGraphNode* child, int remainingDepth);

	/* Returns child set of a graph node */
	std::set<SgGraphNode*> getChildrenSet(SgGraphNode* parent);
	/* Returns the parent set of a graph node */
	std::set<SgGraphNode*> getParentSet(SgGraphNode* child);

	/* Dump the current function declaration -> call graph node mapping */
	void dumpCallGraphMapping(bool keysOnly = true);

	/* XXX needs better name; Calculates the #calls for a method, including all callers */
	long long approximateNumberOfCalls(SgFunctionDeclaration* callee);

	int getNumberOfIncomingEdges(SgGraphNode* node);
	int getNumberOfOutgoingEdges(SgGraphNode* node);

 private:
	/* the callGraph corresponding to an AST */
	SgIncidenceDirectedGraph* callGraph;
	/* a mapping from _first nondefining_ FunctionDeclarations to GraphNodes of the callGraph */
	boost::unordered_map<SgFunctionDeclaration*, SgGraphNode*> nodeMap;

	/*
	 * stores how often a method/callee is called by which caller
	 * callee -> (caller -> #calls)
	 */
	CalleeToCallerMap* callsByCaller;

	boost::unordered_set<SgFunctionDeclaration*> inProgress;

	/* multiplier, how often a node of the callgraph is potentially called */
	boost::unordered_map<SgFunctionDeclaration*, long long> nodeMultiplier;
};
}
#endif	// CALLGRAPHMANAGER_H
