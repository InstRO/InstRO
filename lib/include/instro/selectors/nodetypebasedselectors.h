#ifndef NODE_TYPE_BASED_SELECTORS_H
#define NODE_TYPE_BASED_SELECTORS_H

#include "../common.h"

namespace InstRO{



/**
 *	\brief Select all Nodes of the specified Rose - AST - Type
 *  \ingroup Selector
 *	\author Roman Ness
 */
class GrammarTraitSelector: public OutOfOrderSelector {
public:
	GrammarTraitSelector(SgProject* project, VariantT variant);

	/** selection is done here by node query */
	void selectionBegin(SgProject* project);

	/** visitor does nothing */
	void visit(SgNode* node) {
	}
protected:
	const VariantT variant;
};

/**
 * \brief Selects all function definitions. \
 * shortcut leads to GrammarTraitSelector.
 * \ingroup Selector
 * \author Roman Ness
 */
class FunctionDefinitionSelector: public GrammarTraitSelector {
public:
	FunctionDefinitionSelector(SgProject* project) :
		GrammarTraitSelector(project, V_SgFunctionDefinition) {
	}
};

/**
 * \brief GlobalScope-Selector: Selects all global scopes, for example for the purpose of adding some declarations there
 *
 * \ingroup Selector
 * \author Roman Ness
 */
class GlobalScopeSelector: public GrammarTraitSelector {
public:
	GlobalScopeSelector(SgProject* project) :
		GrammarTraitSelector(project, V_SgGlobal) {
	}
};


}

#endif
