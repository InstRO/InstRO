#include <instro/selectors/callpathselector.h>

#define VERBOSE 0

using namespace InstRO;

/**
	* \brief Constructor when used with a starting statement
	* \param project The project to be traversed
	* \param start The starting Statement in the AST
	* \param maxDepth The relative max depth of selected functions in the call graph
	*/
CallPathSelector::CallPathSelector(SgProject* project, SgStatement* start, int maxDepth)
		: SelectorBasedSelector(project, 0) {
	init(project, maxDepth);

	startingStatements.insert(start);
}

/**
	* \brief Constructor when used with another selector as input
	* \param project The project to be traversed
	* \param selectorId The Id of the other (input) selector
	* \param maxDepth The relative max depth of selected functions in the call graph
	*/
CallPathSelector::CallPathSelector(SgProject* project, Selector* subSelector, int maxDepth)
		: SelectorBasedSelector(project, subSelector) {
	init(project, maxDepth);
	// Starting statements are added by the visitor
}

/**
 * \brief Initializes the call graph manager
 * \param project The project to be traversed
 * \param maxDepth The relative max depth of selected functions in the call graph
 */
void CallPathSelector::init(SgProject* project, int maxDepth) {
	this->maxDepth = maxDepth;

	// Initiate CallGraphManager
	cgManager = new CallGraphManager(project);
}

/**
 * \brief Get starting function declarations
 */
void CallPathSelector::findStartingNodes() { startingDecls = getEnclosingDeclarations(); }

/**
	* \brief Find call graph ancestors for all starting function declarations
	*/
void CallPathSelector::findNodesToSelect() {
	typedef std::set<SgFunctionDeclaration*>::iterator FDeclIt;
	for (FDeclIt it = startingDecls.begin(); it != startingDecls.end(); it++) {
		// Find the entry point in the call graph
		SgGraphNode* entryPoint = cgManager->getCallGraphNode(*it);

		if (entryPoint == NULL) {
#if VERBOSE
			std::cout << "Found no call graph node for: " << (*it)->get_name() << std::endl;
#endif
			continue;
		}

		// Gather the ancestors
		std::set<SgGraphNode*> ancestors = cgManager->getAncestorsTillDepth(entryPoint, maxDepth);
		nodesToSelect.insert(ancestors.begin(), ancestors.end());
		// insert self
		nodesToSelect.insert(entryPoint);
	}

	// CallGraphManager is no longer needed
	delete cgManager;
}

/**
	* \brief select all previously marked nodes via select(..)
	*/
void CallPathSelector::selectFoundNodes() {
	for (std::set<SgGraphNode*>::iterator it = nodesToSelect.begin(); it != nodesToSelect.end(); it++) {
		SgFunctionDeclaration* funcDecl = isSgFunctionDeclaration((*it)->get_SgNode());

		// ignore declarations without definitions
		if (funcDecl->get_definingDeclaration() == 0) {
#if VERBOSE
			std::cout << funcDecl->unparseToString()
								<< " - will not be selected because a defining declaration was not found." << std::endl;
#endif

			continue;
		}
		// get the definition of the defining declaration (to avoid selecting nondef. declarations)
		SgFunctionDefinition* funcDef = ASTHelper::getDefiningDeclaration(funcDecl)->get_definition();

		select(funcDef);
	}
}
