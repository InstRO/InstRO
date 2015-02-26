#ifndef CYG_PROFILE_FUNCTION_ADAPTER__
#define CYG_PROFILE_FUNCTION_ADAPTER__

#include "GenericFunctionAdapterBuilder.h"

namespace InstRO {

// Forward declaration for member-variables
class Builder;

/**
 * \brief This AdapterBuilder build a Cyg-profile-function instrumentation by inserting function calls to the _cyg_function_entry / _cyg_function_exit routines.
 *
 *
 * FIXME 2013-10-08 JP: The adapter builder inserts unions, since some builder functions changed semantics from edg3-Rose to edg4x-Rose, this
 * code crashes sometimes or doesn't produce allowed c++ output.
 * XXX 2013-10-26 JP: At the moment this adapterbuilder doesnt really prepare the AST for instrumentation, since the function pointer in the inserted cyg_profile calls
 * will always have the value 42. This is just to overcome the beforehand mentioned problem.
 *
 * \ingroup Adapterbuilder
 * \author InstRO
 */
class CygProfileFunctionAdapterBuilder: public GenericFunctionAdapterBuilder {
public:
	/** \name Constructor/Destructor */
	/** \{ */
	//CygProfileFunctionAdapterBuilder();
	CygProfileFunctionAdapterBuilder(Selector* sel);
	~CygProfileFunctionAdapterBuilder();
	/** \} */

	// 2013-10-21 JP: New interface
//	void buildForNode(SgNode* node);

protected:
	void setUpFunctionDefinitionInstrumentationEnvironment(SgFunctionDefinition* funcDef);
	SgExprStatement* createEntryFunctionExpressionStatement(SgScopeStatement* scope);
	SgExprStatement* createExitFunctionExpressionStatement(SgScopeStatement* scope);
};

}
#endif
