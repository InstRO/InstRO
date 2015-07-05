#ifndef CALLPATHSELECTOR_H
#define CALLPATHSELECTOR_H

#include "../common.h"
#include "selectorbasedselector.h"
#include "../utility/callgraphmanager.h"
#include "../utility/asthelper.h"

namespace InstRO {

/**
 * \brief This selector marks call graph ancestors of the starting node, \
 * selects FunctionDefinitions
 * \author Roman Ness
 */
class CallPathSelector : public SelectorBasedSelector {
 public:
	CallPathSelector(SgProject* project, SgStatement* start, int maxDepth);

	CallPathSelector(SgProject* project, Selector* subSelector, int maxDepth);

	~CallPathSelector() {}

 private:
	/** call graph ancestors up to this depth are selected */
	int maxDepth;
	/** manages (interaction with) the call graph */
	CallGraphManager* cgManager;

	/* starting function declarations for the analysis */
	std::set<SgFunctionDeclaration*> startingDecls;
	/** holds the nodes that shall be selected */
	std::set<SgGraphNode*> nodesToSelect;

	/** initialize members */
	void init(SgProject* project, int maxDepth);

	/* gather starting function declarations */
	void findStartingNodes();
	/** find ancestors of all starting nodes */
	void findNodesToSelect();
	/** select the nodes that have been added to nodesToMark */
	void selectFoundNodes();
};
}

#endif	// CALLDEPHSELECTOR_H
