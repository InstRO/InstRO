#include "instro/selectors/nodetypebasedselectors.h"

#define VERBOSE 0

using namespace InstRO;

/** Select all Nodes of the specified type */
GrammarTraitSelector::GrammarTraitSelector(SgProject* project, VariantT variant)
: variant(variant) { }

/**
 * \brief Queries the whole project for nodes of the searched type
 * \param project The root of the project
 */
void GrammarTraitSelector::selectionBegin(SgProject* project) {

	Rose_STL_Container<SgNode*> nodes = SageInterface::querySubTree<SgNode>(project, variant);

	for(Rose_STL_Container<SgNode*>::iterator it=nodes.begin(); it!=nodes.end(); it++) {
		this->select(*it);

// internal debugging
#if VERBOSE > 3
		std::cout << "GT selected: " << SageInterface::get_name(*it) << std::endl;
#endif
	}
}
