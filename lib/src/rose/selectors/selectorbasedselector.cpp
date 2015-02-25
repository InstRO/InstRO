#include <instro/selectors/selectorbasedselector.h>

#define VERBOSE 0

using namespace InstRO;

SelectorBasedSelector::SelectorBasedSelector(SgProject* project, Selector* subSelector)
:subSelector(subSelector) {
	// Starting nodes are added by the visitor
}


/**
  * \brief Finds the enclosing declaration if the node was selected by the other selector.
  * \param node The visited note of the AST
  */
void SelectorBasedSelector::visit(SgNode* node) {

	if(subSelector) {
		// if this node was selected by the other selector
		if( ASTAttributeContainer::getASTAttributeContainer(node)->hasSelAttrib(subSelector->getId()) ) {

			SgStatement* stmt = isSgStatement(node);
			if(stmt) {	// just to make sure
				startingStatements.insert(stmt);
			}
		}
	}
}

/**
  * \brief Delegates further analysis after initialization through the visitor \
  * (multiple methods to prepare for further abstraction)
  * \param project The project that was traversed (not used)
  */
void SelectorBasedSelector::selectionEnd(SgProject* project) {

	findStartingNodes();

	findNodesToSelect();

	selectFoundNodes();
}

/**
 * \brief Get the enclosing function declarations of all found starting statements
 */
std::set<SgFunctionDeclaration*> SelectorBasedSelector::getEnclosingDeclarations() {
	std::set<SgFunctionDeclaration*> funcDecls;

	typedef std::set<SgStatement*>::iterator StmtIt;
	for(StmtIt it=startingStatements.begin(); it!=startingStatements.end(); it++) {

		SgFunctionDeclaration *fDecl = SageInterface::getEnclosingFunctionDeclaration((*it), true);
#if VERBOSE > 3
		std::cout << "SelectorBasedSelector::" << __FUNCTION__ << " : found " << fDecl->class_name() << ":\n" << fDecl << " : " << fDecl->get_name() << std::endl;
#endif
		if(fDecl) {
			funcDecls.insert(fDecl);
		}
	}
	return funcDecls;
}

void SelectorBasedSelector::getSubSelectors(std::list<Selector*>& subSelectors) {
	subSelectors.push_back(subSelector);
}
