#include <instro/selectors/calldepthselector.h>

// verbose output for debugging
#define VERBOSE 0

using namespace InstRO;

/**
	* \brief Constructor when used with a starting statement
	* \param project The project to be traversed
	* \param start The starting statement in the AST
	* \param maxDepth The relative max depth of selected functions in the call graph
	*/
CallDepthSelector::CallDepthSelector(SgProject* project, SgStatement* start, int maxDepth, bool precisionMode)
		: SelectorBasedSelector(project, 0) {
	init(project, maxDepth, precisionMode);

	startingStatements.insert(start);
}

/**
	* \brief Constructor when used with another selector as input
	* \param project The project to be traversed
	* \param selectorId The Id of the other (input) selector
	* \param maxDepth The relative max depth of selected functions in the call graph
	*/
CallDepthSelector::CallDepthSelector(SgProject* project, Selector* subSelector, int maxDepth, bool precisionMode)
		: SelectorBasedSelector(project, subSelector) {
	init(project, maxDepth, precisionMode);
	// Starting statements are added by the visitor
}

/**
 * \brief Initializes the call graph manager
 * \param project The project to be traversed
 * \param maxDepth The relative max depth of selected functions in the call graph
 */
void CallDepthSelector::init(SgProject* project, int maxDepth, bool precisionMode) {
	this->maxDepth = maxDepth;
	this->precisionMode = precisionMode;

	// Initiate CallGraphManager
	cgManager = new CallGraphManager(project);
}

void CallDepthSelector::findStartingNodes() {
	typedef std::set<SgStatement*>::iterator StmtIt;

	if (precisionMode) {
/* PRECISION MODE: goes inside the starting statement if possible */
#if VERBOSE
		std::cout << "Using CallDepthManager in Precision Mode" << std::endl;
#endif

		for (StmtIt it = startingStatements.begin(); it != startingStatements.end(); it++) {
			getDeclarationsOfFunctionCallExpressions(*it);
		}

	} else {
/* COARSE MODE: just get the enclosing functionDeclaration if there is one */
#if VERBOSE
		std::cout << "Using CallDepthManager in Coarse Mode" << std::endl;
#endif

		for (StmtIt it = startingStatements.begin(); it != startingStatements.end(); it++) {
			startingDecls.insert(SageInterface::getEnclosingFunctionDeclaration(*it, true));
		}
	}

#if VERBOSE
	typedef std::set<SgFunctionDeclaration*>::iterator FDeclIt;
	for (FDeclIt it = startingDecls.begin(); it != startingDecls.end(); it++) {
		std::cout << "#Declaration: " << ASTHelper::nondefUnparseToString(*it) << std::endl;
	}
#endif
}

/**
	* \brief Finds all FunctionCallExpressions in a subtree, then returns the corresponding Function Declarations
	* \param start a node of the AST, root for the subtree query
	*/
void CallDepthSelector::getDeclarationsOfFunctionCallExpressions(SgStatement* start) {
	// find all FunctionCallExp in the subtree
	std::vector<SgFunctionCallExp*> fcExp = SageInterface::querySubTree<SgFunctionCallExp>(start, V_SgFunctionCallExp);

	std::set<SgFunctionDeclaration*> funcDecls;
	for (std::vector<SgFunctionCallExp*>::iterator it = fcExp.begin(); it != fcExp.end(); it++) {
		// all items can only be FunctionCallExp
		startingDecls.insert((*it)->getAssociatedFunctionDeclaration());
	}
}

void CallDepthSelector::findNodesToSelect() {
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

		std::set<SgGraphNode*> children = cgManager->getDescendantsTillDepth(entryPoint, maxDepth);
		nodesToSelect.insert(children.begin(), children.end());

		// insert self
		nodesToSelect.insert(entryPoint);
	}
}

/**
	* \brief select all previously marked nodes via select(..)
	*/
void CallDepthSelector::selectFoundNodes() {
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

		Selector::select(funcDef);
	}
}
