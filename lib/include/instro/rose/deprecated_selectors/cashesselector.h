#ifndef CASHESSELECTOR_H
#define CASHESSELECTOR_H

#include "../common.h"
#include "selectorbasedselector.h"
#include "../utility/callgraphmanager.h"
#include "../utility/asthelper.h"

namespace InstRO {

/**
 * \brief This selector marks call graph parents of all starting nodes \
 * that have two or more parents.
 *
 * selects FunctionDefinitions
 * \author Roman
 * \author Jan-Patrick Lehr
 */
class CashesSelector : public SelectorBasedSelector {
 public:
	CashesSelector(SgProject* project, SgStatement* start);

	CashesSelector(SgProject* project, Selector* subSelector);

	~CashesSelector() {}

 private:
	/** manages (interaction with) the call graph */
	CallGraphManager* cgManager;

	/* starting function declarations for the analysis */
	std::set<SgFunctionDeclaration*> startingDecls;
	/** holds the nodes that shall be selected */
	std::set<SgGraphNode*> nodesToSelect;

	/* gather starting function declarations */
	void findStartingNodes();
	/** find ancestors of all starting nodes */
	void findNodesToSelect();
	/** select the nodes that have been added to nodesToMark */
	void selectFoundNodes();
};
}

#endif	// CASHESSELECTOR_H
