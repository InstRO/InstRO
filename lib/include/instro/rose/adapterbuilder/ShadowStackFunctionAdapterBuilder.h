#ifndef SHADOW_STACK_FUNCTION_ADAPTER_BUILDER_H_
#define SHADOW_STACK_FUNCTION_ADAPTER_BUILDER_H_

#include "GenericFunctionAdapterBuilder.h"
#include "../utility/functionidentification.h"

namespace InstRO {

/***
 * \brief Inserts calls for stack shadowing into the code
 *
 * Adds calls to
 * _instroPushIdentifier(functionIdentifier, threadIndentifier) - when ever a instrumented function is entered
 * and
 * _instroPopIdentifier(threadIdentifier) - when ever a instrumented function is left
 * to the instrumented function definition.
 *
 * In the current implementation the threadIdentifier is determined using the pthread_self function out of pthread.h
 * It is possible to change that later on, since the InstROBuilder functions expect a SgExpression, which the user can
 *define.
 * The pthread.h file is included at the beginning of the source file.
 *
 * TODO 2013-11-08 JP: Consider different ways of generating function identification (pseudo ids, function pointers,
 *etc)
 * TODO 2013-11-08 JP: How about switching between different modes of thread identification (pthread.h, c++11, none,
 *others?)?
 * TODO 2013-11-08 JP: Add a method for stack initialization here, not in the library, that it does not get called on
 *every function entry
 *
 * \author Jan - Patrick Lehr
 * \ingroup Adapterbuilder
 */
class ShadowStackFunctionAdapterBuilder : public GenericFunctionAdapterBuilder {
 public:
	ShadowStackFunctionAdapterBuilder(Selector* selector, bool usePthreadHeader = false);
	~ShadowStackFunctionAdapterBuilder();

 protected:
	void setUpFunctionDefinitionInstrumentationEnvironment(SgFunctionDefinition* funcDef);
	SgExprStatement* createEntryFunctionExpressionStatement(SgScopeStatement* scope);
	SgExprStatement* createExitFunctionExpressionStatement(SgScopeStatement* scope);

 private:
	SgBasicBlock* createCodeBlockForShadowStackInitialization();

	FunctionIdentificationTable identifierTable;

	bool usePthreadHeader;
	bool isStackInitialized;
	bool pushFunctionHasBeenDeclared;
	bool popFunctionHasBeenDeclared;
	bool pthreadHeaderHasBeenIncluded;
};
}
#endif
