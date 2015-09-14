#ifndef SELECTORBASEDSELECTOR_H_
#define SELECTORBASEDSELECTOR_H_

#include "../common.h"

namespace InstRO {

/**
 * \brief Selectors inherited from this abstract selector receive another selector as input
 * \author Roman Neß
 */
class SelectorBasedSelector : public OutOfOrderSelector {
 public:
	SelectorBasedSelector(SgProject* project, Selector* otherSelector);
	~SelectorBasedSelector() {}
	/** gather starting points with visitor pattern */
	void visit(SgNode* node);
	/** called immediately after visitor */
	void selectionEnd(SgProject* project);

	/** fills the list with sub-selectors */
	void getSubSelectors(std::list<Selector*>& subSelectors);

 private:
	/** gather starting nodes */
	virtual void findStartingNodes() = 0;
	/** gather nodes that shall be selected later */
	virtual void findNodesToSelect() = 0;
	/** select the nodes */
	virtual void selectFoundNodes() = 0;

 protected:
	/** the selector used as Input */
	Selector* subSelector;
	/** holds starting points for inheriting classes */
	std::set<SgStatement*> startingStatements;

	/** get enclosing function declarations of all starting statements */
	std::set<SgFunctionDeclaration*> getEnclosingDeclarations();
};
}

#endif /* SELECTORBASEDSELECTOR_H_ */
