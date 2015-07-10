#include <instro/selectors/cashesselector.h>

#define VERBOSE 2

using namespace InstRO;

/**
	* \brief Constructor when used with a starting statement
	* \param project The project to be traversed
	* \param start The starting Statement in the AST
	*/
CashesSelector::CashesSelector(SgProject* project, SgStatement* start) : SelectorBasedSelector(project, NULL) {
	// Initiate CallGraphManager
	cgManager = new CallGraphManager(project);

	startingStatements.insert(start);
}

/**
	* \brief Constructor when used with another selector as input
	* \param project The project to be traversed
	* \param selectorId The Id of the other (input) selector
	*/
CashesSelector::CashesSelector(SgProject* project, Selector* subSelector)
		: SelectorBasedSelector(project, subSelector) {
	// Initiate CallGraphManager
	cgManager = new CallGraphManager(project);
	// Starting statements are added by the visitor
}

/**
 * \brief Get starting function declarations
 */
void CashesSelector::findStartingNodes() { startingDecls = getEnclosingDeclarations(); }

/**
	* \brief Find call graph ancestors for all starting function declarations
	*/
void CashesSelector::findNodesToSelect() {
	typedef std::set<SgFunctionDeclaration*>::iterator FDeclIt;
	for (FDeclIt it = startingDecls.begin(); it != startingDecls.end(); it++) {
		// Find the entry point in the call graph
		SgGraphNode* entryPoint = cgManager->getCallGraphNode(*it);

		// if more than two parents
		if (cgManager->getNumberOfIncomingEdges(entryPoint) < 2)
			continue;

		if (entryPoint == NULL) {
#if VERBOSE
			std::cout << "Found no call graph node for: " << (*it)->get_name() << std::endl;
#endif
			continue;
		}

#if VERBOSE > 1
		std::cout << "Adding ancestors for: " << (*it)->get_name() << std::endl;
#endif

		// Gather the ancestors
		std::set<SgGraphNode*> ancestors = cgManager->getParentSet(entryPoint);
		nodesToSelect.insert(ancestors.begin(), ancestors.end());
		// insert self
	}

	// CallGraphManager is no longer needed
	delete cgManager;
}

/**
	* \brief select all previously marked nodes via select(..)
	*/
void CashesSelector::selectFoundNodes() {
#if VERBOSE > 2
	std::cout << "Selecting for list of " << nodesToSelect.size() << std::endl;
#endif
	for (std::set<SgGraphNode*>::iterator it = nodesToSelect.begin(); it != nodesToSelect.end(); it++) {
		SgFunctionDeclaration* funcDecl = isSgFunctionDeclaration((*it)->get_SgNode());

		// ignore declarations without definitions
		if (funcDecl->get_definingDeclaration() == 0) {
#if VERBOSE > 3
			std::cout << ASTHelper::nondefUnparseToString(funcDecl)
								<< " - will not be selected because a defining declaration was not found." << std::endl;
#endif

			continue;
		}
		// get the definition of the defining declaration (to avoid selecting nondef. declarations)
		SgFunctionDefinition* funcDef = ASTHelper::getDefiningDeclaration(funcDecl)->get_definition();
#if VERBOSE > 1
		std::cout << "Selecting: " << funcDecl->get_file_info()->get_filenameString() << ": " << funcDecl->get_name()
							<< std::endl;
#endif
		select(funcDef);
	}
}
