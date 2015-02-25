#include <instro/adapterbuilder/VampirTraceAdapterBuilder.h>

using namespace InstRO;

#ifdef DEBUG
#define DEBUGENTRY std::cout <<"InstRO Debug: Enter VampirTraceAdapterBuilder::"<< __FUNCTION__ << std::endl;
#define DEBUGEXIT std::cout << "InstRO Debug: Exit VampirTraceAdapterBuilder::" << __FUNCTION__ << std::endl;
#define VERBOSE 0
#else
#define DEBUGENTRY
#define DEBUGEXIT
#endif

VampirTraceAdapterBuilder::VampirTraceAdapterBuilder(Selector * decidingSelector) :
		GenericFunctionAdapterBuilder(decidingSelector) {
	writeCalled = false;
}

VampirTraceAdapterBuilder::~VampirTraceAdapterBuilder() {
	// in case was not called explicitly. Just to make sure.
	if (!writeCalled) {
		writeNamefile();
	}
}

void VampirTraceAdapterBuilder::writeNamefile() {
	identifierTable.writeNamefile();
}

SgExprStatement* VampirTraceAdapterBuilder::createEntryFunctionExpressionStatement(SgScopeStatement* scope){
#ifdef DEBUG
	DEBUGENTRY
#endif
	SgExpression* firstArgument = 0;
	SgExpression* secondArgument = 0;

	secondArgument = InstROBuilder::buildNullPointer();
	SgExpression* intExpression = SageBuilder::buildIntVal(identifierTable.getLabel());
	SgType* voidType = SageBuilder::buildVoidType();

	firstArgument = SageBuilder::buildCastExp(intExpression, SageBuilder::buildPointerType(voidType));

	SgExprStatement *functionCallEnter = InstROBuilder::buildFunctionCallEnter (scope, firstArgument, secondArgument);
	declareEntryFunction(SageInterface::getGlobalScope(scope));
#ifdef DEBUG
	DEBUGEXIT
#endif
	return functionCallEnter;

}
SgExprStatement* VampirTraceAdapterBuilder::createExitFunctionExpressionStatement(SgScopeStatement* scope){
#ifdef DEBUG
	DEBUGENTRY
#endif
	SgExpression* firstArgument = 0;
	SgExpression* secondArgument = 0;

//	secondArgument = builder->buildNullPointer();
	secondArgument = InstROBuilder::buildNullPointer();
	SgExpression* intExpression = SageBuilder::buildIntVal(identifierTable.getLabel());
	SgType* voidType = SageBuilder::buildVoidType();

	firstArgument = SageBuilder::buildCastExp(intExpression, SageBuilder::buildPointerType(voidType));

//	SgExprStatement *functionCallExit = builder->buildFunctionCallExit (scope, firstArgument, secondArgument);
	SgExprStatement *functionCallExit = InstROBuilder::buildFunctionCallExit (scope, firstArgument, secondArgument);
	declareExitFunction(SageInterface::getGlobalScope(scope));
#ifdef DEBUG
	DEBUGEXIT
#endif
	return functionCallExit;
}

void VampirTraceAdapterBuilder::setUpFunctionDefinitionInstrumentationEnvironment(SgFunctionDefinition* funcDef){
	identifierTable.insertIdentifier(funcDef);
}
