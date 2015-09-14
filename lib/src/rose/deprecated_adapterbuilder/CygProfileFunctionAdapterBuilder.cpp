#include <instro/adapterbuilder/CygProfileFunctionAdapterBuilder.h>

#ifdef DEBUG
#define DEBUGENTRY std::cout << "InstRO Debug: Enter " << __FUNCTION__ << std::endl;
#define DEBUGEXIT std::cout << "InstRO Debug: Exit " << __FUNCTION__ << std::endl;
#define VERBOSE 0
#else
#define DEBUGENTRY
#define DEBUGEXIT
#endif

using namespace InstRO;

void CygProfileFunctionAdapterBuilder::setUpFunctionDefinitionInstrumentationEnvironment(
		SgFunctionDefinition* funcDef) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	// do some pre-processing like instrumentation union creation etc.
	std::cerr << "INSTRO ERROR: THE CYG PROFILE ADAPTER BUILDER IS NOT FULLY OPERATIONAL!\nAll Function Pointers point "
							 "to address 42. We are working on this!" << std::endl;

	SgVariableDeclaration* unionMemFuncPtr;
	SgVariableDeclaration* unionVoidPtr;

	if (isSgMemberFunctionDeclaration(funcDef->get_declaration()) != NULL)
		InstROBuilder::buildUnion(funcDef, unionMemFuncPtr, unionVoidPtr);

#ifdef DEBUG
	DEBUGEXIT
#endif
}
SgExprStatement* CygProfileFunctionAdapterBuilder::createEntryFunctionExpressionStatement(SgScopeStatement* scope) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	SgExpression* firstArgument = 0;
	SgExpression* secondArgument = 0;

	secondArgument = InstROBuilder::buildNullPointer();
	SgExpression* intExpression = SageBuilder::buildIntVal(42);
	SgType* voidType = SageBuilder::buildVoidType();

	firstArgument = SageBuilder::buildCastExp(intExpression, SageBuilder::buildPointerType(voidType));

	SgExprStatement* functionCallEnter = InstROBuilder::buildFunctionCallEnter(scope, firstArgument, secondArgument);
	declareEntryFunction(SageInterface::getGlobalScope(scope));
#ifdef DEBUG
	DEBUGEXIT
#endif
	return functionCallEnter;
}
SgExprStatement* CygProfileFunctionAdapterBuilder::createExitFunctionExpressionStatement(SgScopeStatement* scope) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	SgExpression* firstArgument = 0;
	SgExpression* secondArgument = 0;

	secondArgument = InstROBuilder::buildNullPointer();
	SgExpression* intExpression = SageBuilder::buildIntVal(42);
	SgType* voidType = SageBuilder::buildVoidType();

	firstArgument = SageBuilder::buildCastExp(intExpression, SageBuilder::buildPointerType(voidType));

	SgExprStatement* functionCallExit = InstROBuilder::buildFunctionCallEnter(scope, firstArgument, secondArgument);
	declareExitFunction(SageInterface::getGlobalScope(scope));
#ifdef DEBUG
	DEBUGEXIT
#endif
	return functionCallExit;
}
