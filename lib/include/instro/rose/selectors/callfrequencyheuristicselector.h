#ifndef CALLFREQUENCYHEURISTICSELECTOR_H
#define CALLFREQUENCYHEURISTICSELECTOR_H

#include "../common.h"

#include "instro/utility/asthelper.h"
#include "instro/utility/callgraphmanager.h"


namespace InstRO {

/**
 * \brief Selector that approximates the #calls for a method.
 * Uses parameters of loops and the call graph as multipliers
 * \author Roman
 */
class CallFrequencyHeuristicSelector : public OutOfOrderSelector {
public:
	CallFrequencyHeuristicSelector(SgProject* project, long defaultCount=23L, long threashold=100L);

	void selectionBegin(SgProject* project);
	void selectionEnd(SgProject* project);

	/* visit all nodes */
	void visit(SgNode *n);


private:
	/* stores all registered function call expressions */
	boost::unordered_set<SgFunctionCallExp*> callExps;
	/* stores all registered function definitions */
	boost::unordered_set<SgFunctionDefinition*> callees;

	/* stores how often each loop is processed approximately */
	boost::unordered_map<SgScopeStatement*, long> loopCounts;
	// 2014-03 JP XXX I changed the visibility here to protected due some testing.
protected:
	/* stores how often a method is called in total */
	boost::unordered_map<SgFunctionDefinition*, long long> callsByFuncDef;
private:
	/*
	 * stores how often a method/callee is called by which caller
	 * callee -> (caller -> #calls)
	 */
	CalleeToCallerMap callsByCaller;

	/* manages (interaction with) the call graph */
	CallGraphManager* cgManager;

	/* default value, when the loop counter cannot be determined */
	long defaultCount;
	/* threshold for methods that shall be selected */
	long threshold;

	/* determine calls by function, then do the selection */
	void prepareAndDoSelection();

	/* approximate how often a certain loop is processed */
	long approximateCount(SgForStatement* forStmt);
	long approximateCount(SgWhileStmt* forStmt);
	long approximateCount(SgDoWhileStmt* forStmt);

	/* get the actual value (number) of an expression */
	long getValue(SgExpression* expr);
	/* shortcut method */
	SgScopeStatement* getEnclosingSgScopeStatement(SgNode* n);

	/* print the loop count heuristically for all registered scopes */
	void dumpLoopCounts();
	/* print all registered function call expressions */
	void dumpFunctionCalls();
	/* print how often which method was called by heuristic */
	void dumpNumberOfCalls();
	/* print how often which method is called (and also which method is the caller) */
	void dumpCallsByCaller();
};

}

#endif // CALLFREQUENCYHEURISTICSELECTOR_H
