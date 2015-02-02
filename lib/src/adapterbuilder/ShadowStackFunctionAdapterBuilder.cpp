#include "instro/adapterbuilder/ShadowStackFunctionAdapterBuilder.h"

#ifdef DEBUG
#define DEBUGENTRY std::cout <<"InstRO Debug: Enter ShadowStackFunctionAdapterBuilder::"<< __FUNCTION__ << std::endl;
#define DEBUGEXIT std::cout << "InstRO Debug: Exit ShadowStackFunctionAdapterBuilder::" << __FUNCTION__ << std::endl;
#define VERBOSE 0
#else
#define DEBUGENTRY
#define DEBUGEXIT
#endif

using namespace InstRO;


ShadowStackFunctionAdapterBuilder::ShadowStackFunctionAdapterBuilder(Selector* selector, bool usePthreadHeader) :
		usePthreadHeader(usePthreadHeader), isStackInitialized(false), pushFunctionHasBeenDeclared(false), popFunctionHasBeenDeclared(false), pthreadHeaderHasBeenIncluded(false), GenericFunctionAdapterBuilder(selector) {

}
ShadowStackFunctionAdapterBuilder::~ShadowStackFunctionAdapterBuilder(){
	identifierTable.writeNamefile();
}
void ShadowStackFunctionAdapterBuilder::setUpFunctionDefinitionInstrumentationEnvironment(SgFunctionDefinition* funcDef) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	// Set up shadow stack
	if (!isStackInitialized) {
		//SgBasicBlock* initBlock = createCodeBlockForShadowStackInitialization();
	}
	// Do we want to insert calls to pthread_self in the measurement hooks?
	if(usePthreadHeader)
		if (!pthreadHeaderHasBeenIncluded) {
			SageInterface::insertHeader("pthread.h", PreprocessingInfo::before, true, SageInterface::getGlobalScope(funcDef));
			pthreadHeaderHasBeenIncluded = true;
		}
	// Inject code to be called when entering function
	// Inject code to be called when leaving function
	// inject code to be able to query from a library
	SgGlobal* globalScope = SageInterface::getGlobalScope(funcDef);
	if (!pushFunctionHasBeenDeclared) {
		declareProfileFunction(InstROBuilder::buildPushIdentifierToStackDeclaration(funcDef), globalScope);
		pushFunctionHasBeenDeclared = true;
	}
	if (!popFunctionHasBeenDeclared) {
		declareProfileFunction(InstROBuilder::buildPopIdentifierFromStackDeclaration(funcDef), globalScope);
		popFunctionHasBeenDeclared = true;
	}
	identifierTable.insertIdentifier(funcDef);
#ifdef DEBUG
	DEBUGEXIT
#endif
}

SgExprStatement* ShadowStackFunctionAdapterBuilder::createEntryFunctionExpressionStatement(SgScopeStatement* scope) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	if (isSgFunctionDefinition(scope->get_parent())) {
		SgFunctionDefinition* fDef = isSgFunctionDefinition(scope->get_parent());

		// we have to have some identifier for this function.
		size_t identifier = identifierTable.getLabel();

		// XXX 2013-11-08 JP: In case of C++11 we can use the std::this_thread::get_id() function to retrieve the id of the running thread.
		// In case of not C++11 we need to utilize some other method. Of interest? http://ubuntuforums.org/showthread.php?t=345317
		// TODO 2013-11-08 JP: What about performance if we always ask for the thread id, instead of one-time query and save thread ids

		// Based on whether we want to use the pthread_self call we build the correct hook
		if(usePthreadHeader){
			SgExpression* pthreadSelfCallExp = SageBuilder::buildFunctionCallExp("pthread_self", SageBuilder::buildOpaqueType("pthread_t", scope), SageBuilder::buildExprListExp(), scope);
#ifdef DEBUG
	DEBUGENTRY
#endif
			return InstROBuilder::buildCallToPushIdentifierToStackFunction(identifier, pthreadSelfCallExp, scope);

		} else {
			// XXX We can recycle this function later on, if we can predetermine the thread identifier!
			// If we have single threaded applications we can avoid additional overhead by using simply a literal
			size_t threadIdentifier = 0;
			SgExpression* threadIdentifierExpr = SageBuilder::buildUnsignedLongLongIntVal(threadIdentifier);
#ifdef DEBUG
	DEBUGENTRY
#endif
			return InstROBuilder::buildCallToPushIdentifierToStackFunction(identifier, threadIdentifierExpr, scope);
		}

	}
#ifdef DEBUG
	DEBUGEXIT
#endif
	return NULL;
}

SgExprStatement* ShadowStackFunctionAdapterBuilder::createExitFunctionExpressionStatement(SgScopeStatement* scope) {
#ifdef DEBUG
	DEBUGENTRY
#endif

	if(usePthreadHeader){
		SgExpression* pthreadSelfCallExp = SageBuilder::buildFunctionCallExp("pthread_self", SageBuilder::buildOpaqueType("pthread_t", scope), SageBuilder::buildExprListExp(), scope);
#ifdef DEBUG
	DEBUGEXIT
#endif
		return InstROBuilder::buildCallToPopIdentifierFromStackFunction(pthreadSelfCallExp, scope);
	} else {
		size_t threadIdentifier = 0;
		SgExpression* threadIdentifierExpr = SageBuilder::buildUnsignedLongLongIntVal(threadIdentifier);
#ifdef DEBUG
	DEBUGEXIT
#endif
		return InstROBuilder::buildCallToPopIdentifierFromStackFunction(threadIdentifierExpr, scope);
	}
#ifdef DEBUG
	DEBUGEXIT
#endif
}

SgBasicBlock* createCodeBlockForShadowStackInitialization() {
#ifdef DEBUG
	DEBUGENTRY
#endif
	// build a new basic block
//	SgBasicBlock* initializationBlock = SageBuilder::buildBasicBlock();

	// create code which allocates a stack and prepares everything

	// create a variable declaration
#ifdef DEBUG
	DEBUGEXIT
#endif
	return NULL;
}
