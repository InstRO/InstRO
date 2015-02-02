#ifndef GENERIC_FUNCTION_ADAPTER_BUILDER_H
#define GENERIC_FUNCTION_ADAPTER_BUILDER_H


#include "../common.h"
#include "../utility/instrobuilder.h"

namespace InstRO {

/**
 * \brief Base class for a _cyg_profile-style function instrumentation
 *
 * To use this class:
 * Derive from this class and override:
 *
 * setUpFunctionDefinitionInstrumentationEnvironment(SgFunctionDefinition* funcDef) - which gets called at each SgFunctionDefinition first
 * createEntryFunctionExpressionStatement(SgScopeStatement* scope) - a call to this function will be inserted at the beginning of the function definition
 * createExitFunctionExpressionStatement(SgScopeStatement* scope) - a call to this function will be inserted at each possible return from the function definition
 *
 * \ingroup Adapterbuilder
 *
 *\author Jan-Patrick Lehr
 */
class GenericFunctionAdapterBuilder : public AdapterBuilder {

public:
		GenericFunctionAdapterBuilder(Selector* decidingSelector);
		virtual void buildForNode(SgNode* node);

		void startModificationPass();

protected:
		// TODO 2013-10-22 JP: Would it be good to have these methods pure virtual?
		virtual void setUpFunctionDefinitionInstrumentationEnvironment(SgFunctionDefinition* funcDef){};
		virtual SgExprStatement* createEntryFunctionExpressionStatement(SgScopeStatement* scope){return NULL;};
		virtual SgExprStatement* createExitFunctionExpressionStatement(SgScopeStatement* scope){return NULL;};
		/** Inserts an "extern 'C'" declaration of the __cyg_profile_enter(...) function at global scope */
		void declareEntryFunction(SgGlobal* globalScope);
		/** Inserts an "extern 'C'" declaration of the __cyg_profile_exit(...) function at global scope */
		void declareExitFunction(SgGlobal* globalScope);

		/** Adds the declaration at global scope - doesn't check for multiple declarations */
		void declareProfileFunction(SgFunctionDeclaration* profFunctionDecl, SgGlobal* globalScope);

private:
		void declareTemplateInstantiation(SgTemplateInstantiationFunctionDecl* funcDecl);
		void declareTemplateInstantiation(SgTemplateInstantiationMemberFunctionDecl* funcDecl);
		void instrumentFunctionDefinition(SgFunctionDefinition* funcDef);
		void instrumentNonGlobalScope(SgScopeStatement* scope);
		SgBasicBlock* encloseInTryBlock(SgScopeStatement * scope);
		void populateCatchAllBlock(SgScopeStatement * scope);
		void instrumentReturnStatements(SgScopeStatement * scope);
		void transformReturn(SgReturnStmt * returnStatement);
		void prependEnterCall(SgScopeStatement * scope);
		void appendExitCall(SgScopeStatement * scope);
		void assertUnparse(SgNode* node);

		bool entryFunctionHasBeenDeclared;
		bool exitFunctionHasBeenDeclared;

};

}

#endif
