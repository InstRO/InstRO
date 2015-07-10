#ifndef CALLDEPTHSELECTOR_H
#define CALLDEPTHSELECTOR_H

#include "../common.h"
#include "selectorbasedselector.h"
#include "../utility/callgraphmanager.h"
#include "../utility/asthelper.h"

namespace InstRO {

/**
 * \brief This selector marks call graph children of the starting node
 * \author Roman Neß
 */
class CallDepthSelector : public SelectorBasedSelector {
 public:
	CallDepthSelector(SgProject* project, SgStatement* start, int maxDepth, bool precisionMode = false);

	CallDepthSelector(SgProject* project, Selector* subSelector, int maxDepth, bool precisionMode = false);

	~CallDepthSelector() { delete cgManager; }

 private:
	/* turn on/off precision mode */
	bool precisionMode;
	/** call graph descendants up to this depth are selected */
	int maxDepth;
	/* manages (interaction with) the call graph */
	CallGraphManager* cgManager;

	/* starting function declarations for the analysis */
	std::set<SgFunctionDeclaration*> startingDecls;
	/* holds the nodes that shall be selected */
	std::set<SgGraphNode*> nodesToSelect;

	/** initialize members */
	void init(SgProject* project, int maxDepth, bool precisionMode);

	/* gather starting function declarations */
	void findStartingNodes();
	/* marks ancestors and the starting node for selection based on the call graph */
	void findNodesToSelect();
	/* select the nodes that have been added to nodesToMark */
	void selectFoundNodes();

	/* gets the declarations of all function calls in a subtree */
	void getDeclarationsOfFunctionCallExpressions(SgStatement* start);
};
}

#endif	// CALLDEPTHSELECTOR_H
