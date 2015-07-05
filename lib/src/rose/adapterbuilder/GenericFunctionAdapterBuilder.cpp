#include "instro/adapterbuilder/GenericFunctionAdapterBuilder.h"

#ifdef DEBUG
#define DEBUGENTRY std::cout << "InstRO Debug: Enter GenericFunctionAdapterBuilder::" << __FUNCTION__ << std::endl;
#define DEBUGEXIT std::cout << "InstRO Debug: Exit GenericFunctionAdapterBuilder::" << __FUNCTION__ << std::endl;
#define VERBOSE 0
#else
#define DEBUGENTRY
#define DEBUGEXIT
#endif

using namespace InstRO;

GenericFunctionAdapterBuilder::GenericFunctionAdapterBuilder(Selector *decidingSelector)
		: AdapterBuilder(decidingSelector), entryFunctionHasBeenDeclared(false), exitFunctionHasBeenDeclared(false) {}

void GenericFunctionAdapterBuilder::startModificationPass() {
	std::vector<SgNode *> nodes = generateSelectedNodeSet();
	std::vector<SgNode *>::iterator nodeIter;
	for (nodeIter = nodes.begin(); nodeIter != nodes.end(); nodeIter++) {
		buildForNode(*nodeIter);
	}
}

void GenericFunctionAdapterBuilder::buildForNode(SgNode *node) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	if (isSgFunctionDefinition(node)) {
		if (isNodeSelected(node)) {
			if (isSgTemplateInstantiationFunctionDecl(node->get_parent())) {
				if (isSgTemplateInstantiationFunctionDecl(node->get_parent())->get_file_info()->isCompilerGenerated()) {
#ifndef INSTRUMENT_COMPILER_INSTANTIATED_TEMPLATES
					return;	// FIXME 2013-10-08 JP: To prevent InstRO from producing invalid c++ code
#endif
#ifdef DEBUG
					std::cout << "InstRO DEBUG: About to enter "
											 "declareTemplateInstantiation(isSgTemplateInstantiationFunctionDecl(node->get_parent()));"
										<< std::endl;
#endif
					declareTemplateInstantiation(isSgTemplateInstantiationFunctionDecl(node->get_parent()));
				}
				assertUnparse(node->get_parent());
			} else if (isSgTemplateInstantiationMemberFunctionDecl(node->get_parent())) {
				if (isSgTemplateInstantiationMemberFunctionDecl(node->get_parent())->get_file_info()->isCompilerGenerated()) {
#ifndef INSTRUMENT_COMPILER_INSTANTIATED_TEMPLATES
					return;	// FIXME 2013-10-08 JP: To prevent InstRO from producing invalid c++ code
#endif
#ifdef DEBUG
					std::cout << "InstRO DEBUG: About to enter "
											 "declareTemplateInstantiation(isSgTemplateInstantiationMemberFunctionDecl(node->get_parent()));"
										<< std::endl;
#endif
					declareTemplateInstantiation(isSgTemplateInstantiationMemberFunctionDecl(node->get_parent()));
				}
				assertUnparse(node->get_parent());
			}

			// 2013/10/07 JP: Due a bug / problem in ROSE unparser, see all three fix me above
			if (isSgTemplateMemberFunctionDeclaration(node->get_parent()) != NULL) {
				std::cout << "WARNING: No changes to method body will be unparsed in node " << node << " !\n"
									<< isSgTemplateMemberFunctionDeclaration(node->get_parent())->get_string() << std::endl;
			}
			// here we have a function definition
			instrumentFunctionDefinition(isSgFunctionDefinition(node));
		}
	}
}

void GenericFunctionAdapterBuilder::declareTemplateInstantiation(SgTemplateInstantiationFunctionDecl *funcDecl) {
#ifdef DEBUG
	DEBUGENTRY
#endif
#if 1
	std::cerr << "INSTRO WARNING: Since a template instantiation gets forward declared, this can lead to invalid c++, "
							 "especially for internals of iostream!" << std::endl;
#endif

	// Place the forward declaration in the global scope in front of the first call to it
	if (funcDecl->get_file_info()->isCompilerGenerated()) {
		// SgFunctionCallExp *firstCall = getFirstCall(funcDecl);
		SgScopeStatement *futureScope;
		SgStatement *uppermostStatement = funcDecl;
#if 0
		if (firstCall != 0) {
//      /*       SgStatement * */ uppermostStatement =	getUppermostStatement (firstCall);

			uppermostStatement = funcDecl->get_templateDeclaration();

			if (uppermostStatement != NULL) {
				futureScope = uppermostStatement->get_scope();
			}
			ROSE_ASSERT (futureScope != NULL);

		} else {
#endif
		std::cout << "InstRO developer notice: Inside declareTemplateInstantiation. Former else part." << std::endl;
		SgSymbol *fSym = funcDecl->search_for_symbol_from_symbol_table();
		ROSE_ASSERT(fSym);
		futureScope = SageInterface::getGlobalScope(SageInterface::findMain(SageInterface::getProject()));
		uppermostStatement = SageInterface::getFirstStatement(futureScope);
		uppermostStatement = SageInterface::findMain(SageInterface::getProject());
		//		}
		futureScope = SageInterface::getFirstGlobalScope(SageInterface::getProject());
		//		SgTemplateInstantiationFunctionDecl *forwardDeclaration = builder->buildForwardDeclaration(funcDecl,
		//futureScope);
		SgTemplateInstantiationFunctionDecl *forwardDeclaration =
				InstROBuilder::buildForwardDeclaration(funcDecl, futureScope);

		// SageInterface::insertStatementBefore (uppermostStatement, forwardDeclaration);
		SageInterface::insertStatementBefore(SageInterface::findMain(SageInterface::getProject()), forwardDeclaration);

#ifdef DEBUG
		DEBUGEXIT
#endif
	}
}

void GenericFunctionAdapterBuilder::declareTemplateInstantiation(SgTemplateInstantiationMemberFunctionDecl *funcDecl) {
#ifdef DEBUG
	DEBUGENTRY
#endif
#if 1
	std::cerr << "INSTRO WARNING: Since a template instantiation gets forward declared, this can lead to invalid c++, "
							 "especially for internals of iostream!" << std::endl;
#endif

#ifdef USE_OLD_INSTRO_IMPLEMENTATION
	// 2013-07-05 JP: We only have to forward declare compiler generated instantiations!
	if (funcDecl->get_file_info()->isCompilerGenerated()) {
		SgStatement *uppermostStatement = funcDecl;	// = getUppermostStatement (firstCall);

		uppermostStatement = funcDecl->get_templateDeclaration();
		SgScopeStatement *futureScope;
		if (uppermostStatement != NULL) {
			futureScope = isSgScopeStatement(uppermostStatement->get_parent()->get_parent()->get_parent());
			//      	  std::cout << "class name: " <<
			//      uppermostStatement->get_parent()->get_parent()->get_parent()->class_name() << std::endl;
		}

		//		SgMemberFunctionDeclaration *forwardDeclaration = builder->buildForwardDeclaration(funcDecl, futureScope);
		SgMemberFunctionDeclaration *forwardDeclaration =
				InstROBuilder::buildForwardDeclaration(funcDecl, futureScope);	// new Interface
#else
	// 2013-10-16 JP: This code uses a function I found in the ROSE doxygen, which should build a non-defining declaration
	// from the defining one.
	// Nevertheless, we have to strip the definition from the defining declaration to get the function to work.
	// After all, it does have the same problems with system includes as our implementation, but it might be a bit more
	// stable?
	SgTemplateInstantiationMemberFunctionDecl *forwardDeclaration = NULL;
	if (funcDecl == funcDecl->get_definingDeclaration()) {
		SgFunctionDefinition *def = funcDecl->get_definition();
		funcDecl->set_definition(NULL);
		forwardDeclaration = SageInterface::buildForwardFunctionDeclaration(funcDecl);
		funcDecl->set_definition(def);
	} else {
		forwardDeclaration = SageInterface::buildForwardFunctionDeclaration(funcDecl);
	}
#endif

		forwardDeclaration->setOutputInCodeGeneration();
		// SageInterface::insertStatementBefore (uppermostStatement, forwardDeclaration);
		//      SageInterface::insertStatementAfter(uppermostStatement, forwardDeclaration);
		// FIXME 2013-10-08 JP: This is a potential bug, as forwards declarations are always inserted directly before main
		SageInterface::insertStatementBefore(SageInterface::findMain(SageInterface::getProject()), forwardDeclaration);
#ifdef USE_OLD_INSTRO_IMPLEMENTATION
	}
#endif

// TODO 2013-10-08 JP: Place the forward declaration in the (global) scope in front of the first call to it
#ifdef DEBUG
	DEBUGEXIT
#endif
}

void GenericFunctionAdapterBuilder::instrumentFunctionDefinition(SgFunctionDefinition *funcDef) {
#ifdef DEBUG
	DEBUGENTRY
#endif

	if ((funcDef->get_declaration()->get_specialFunctionModifier().isConstructor()) ||
			(funcDef->get_declaration()->get_specialFunctionModifier().isDestructor())) {
		// If this is a constructor / destructor just leave it
		return;
	}

	setUpFunctionDefinitionInstrumentationEnvironment(funcDef);

	SgScopeStatement *toInstrument = funcDef->get_body();

	instrumentNonGlobalScope(toInstrument);
#ifdef DEBUG
	DEBUGEXIT
#endif
}

void GenericFunctionAdapterBuilder::instrumentNonGlobalScope(SgScopeStatement *scope) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	// Only take care of exceptions if we  are in a c++-file
	if (SageInterface::getEnclosingNode<SgProject>(scope)->get_Cxx_only()) {
		SgBasicBlock *catchAllBlock = encloseInTryBlock(scope);
		populateCatchAllBlock(catchAllBlock);
	}

	// XXX 2013-10-16 JP: Potentially we can use the "instrumentEndOfFunction(...)" function provided by the SageInterface
	// here instead of our implementation?
	instrumentReturnStatements(scope);

	prependEnterCall(scope);
#ifdef DEBUG
	std::cout << "InstRO DEBUG: Scope before appendExitCall(): " << scope->unparseToString() << std::endl;
#endif
	appendExitCall(scope);
#ifdef DEBUG
	std::cout << "InstRO DEBUG: Scope after appendExitCall(): " << scope->unparseToString() << std::endl;
#endif
	scope->setOutputInCodeGeneration();
#ifdef DEBUG
	DEBUGEXIT
#endif
}

void GenericFunctionAdapterBuilder::instrumentReturnStatements(SgScopeStatement *scope) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	/* First of all we get all the return statements in the given definition */
	Rose_STL_Container<SgNode *> returnStatementList = NodeQuery::querySubTree(scope, V_SgReturnStmt);

	///
	SgFunctionDefinition *fD = SageInterface::getEnclosingFunctionDefinition(scope, true);

	SgFunctionType *functionType = fD->get_declaration()->get_type();
	SgType *functionReturnType = functionType->get_return_type();

	for (Rose_STL_Container<SgNode *>::iterator i = returnStatementList.begin(); i != returnStatementList.end(); i++) {
#if 0
		/*
		 * Problematic types are: SgNamedTypes, SgReferenceTypes (if there are SgNamedTypes behind that),
		 * SgPointerType (if there are SgNamedTypes behind)
		 */
		if(isSgTypedefType(functionReturnType) != NULL)
		continue;
		if(isSgReferenceType(functionReturnType) != NULL)
		continue;
		if(isSgClassType(functionReturnType) != NULL)
		continue;
		if(isSgNamedType(functionReturnType) != NULL)
		continue;
		if(isSgPointerType(functionReturnType) != NULL)
		continue;
#endif
		// Only transform return statements different from void
		if (isSgTypeVoid(functionReturnType) == NULL)
			transformReturn(isSgReturnStmt(*i));

		/* At this point functionDefinition is sure to be a function definition. We can use this definition to build the
		 * argument list for the call to __cyg_profile_func_exit */
		SgExprStatement *functionCallExit = createExitFunctionExpressionStatement(scope);
		SageInterface::insertStatementBefore(isSgStatement(*i), functionCallExit);
	}
#ifdef DEBUG
	DEBUGEXIT
#endif
}

// TODO 2013-10-08 JP: Only enclose a certain thing in a try block if an exception could be thrown
SgBasicBlock *GenericFunctionAdapterBuilder::encloseInTryBlock(SgScopeStatement *scope) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	//	SgTryStmt *enclosingTry = builder->buildTryStmt(new SgBasicBlock, SageInterface::getScope(scope->get_parent()));
	SgTryStmt *enclosingTry = InstROBuilder::buildTryStmt(new SgBasicBlock, SageInterface::getScope(scope->get_parent()));

	//	SgBasicBlock *tryBlock = builder->buildBlock(enclosingTry);
	SgBasicBlock *tryBlock = InstROBuilder::buildBlock(enclosingTry);

	SageInterface::replaceStatement(scope, tryBlock);

	delete enclosingTry->get_body();
	enclosingTry->set_body(scope);

	// Let the builder build an empty block to return it later on
	//	SgBasicBlock *catchBlock = builder->buildBlock();
	SgBasicBlock *catchBlock = InstROBuilder::buildBlock();

	//	SgCatchOptionStmt *catchStatement = builder->buildCatchAll(catchBlock, enclosingTry);
	SgCatchOptionStmt *catchStatement = InstROBuilder::buildCatchAll(catchBlock, enclosingTry);

	enclosingTry->append_catch_statement(catchStatement);
#ifdef DEBUG
	DEBUGEXIT
#endif
	return catchBlock;
}

void GenericFunctionAdapterBuilder::populateCatchAllBlock(SgScopeStatement *scope) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	// Build a call to the profiling function and a rethrow
	//	SgExprStatement *rethrowStmt = builder->buildRethrowStatement();
	SgExprStatement *rethrowStmt = InstROBuilder::buildRethrowStatement();
	SgExprStatement *profilingFunctionCall = createExitFunctionExpressionStatement(scope);

	// Add both statements to the catch-all-block
	// Be sure to add them 'upside down'
	SageInterface::appendStatement(rethrowStmt, scope);
	SageInterface::appendStatement(profilingFunctionCall, scope);
#ifdef DEBUG
	DEBUGEXIT
#endif
}

void GenericFunctionAdapterBuilder::transformReturn(SgReturnStmt *returnStatement) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	bool returnsSimpleValue =
			(isSgValueExp(returnStatement->get_expression()) != 0) || (isSgVarRefExp(returnStatement->get_expression()) != 0);
	/* We should also not try to move the value if the function returns a void, i.e. if we have a "return;" */
	bool returnsVoid =
			(returnStatement->get_expression() ==
			 0);	// XXX Could this statement ever be evaluated true, since we check in instrumentReturnStatements ?

	if (returnsSimpleValue == false && returnsVoid == false) {
		/* At first we need to store the return value in a temporary variable */
		/* We can use the definition to get to the declaration and thus to the return type of the function */
		SgScopeStatement *scope = NULL;
		SgNode *node = returnStatement;
		while (!(scope = isSgScopeStatement(node))) {
			// maybe print the tree uprising
			node = node->get_parent();
		}
		//            std::cout << "scope now is: " << scope->unparseToString() << std::endl;

		// SgVariableDeclaration* temporaryVariableDeclaration =
		// builder->buildTemporaryVariable(SageInterface::getScope(returnStatement), returnStatement);
		// SgVariableDeclaration* temporaryVariableDeclaration =
		// BuilderInterface::buildTemporaryVariable(SageInterface::getScope(returnStatement), returnStatement);
		//		SgVariableDeclaration *temporaryVariableDeclaration = builder->buildTemporaryVariable(scope, returnStatement);
		SgVariableDeclaration *temporaryVariableDeclaration = InstROBuilder::buildTemporaryVariable(scope, returnStatement);
		//       std::cout << "/*\n(a) Temporary ( " << temporaryVariableDeclaration << " ) in " <<
		//       temporaryVariableDeclaration->get_parent()<< "\n*/" << std::endl;
		SageInterface::insertStatementBefore(returnStatement, temporaryVariableDeclaration);

		/* Build the statement referencing the variable */
		SgVarRefExp *variableReference = SageBuilder::buildVarRefExp(temporaryVariableDeclaration);
		returnStatement->set_expression(variableReference);
		returnStatement->setOutputInCodeGeneration();
	}
#ifdef DEBUG
	DEBUGEXIT
#endif
}

void GenericFunctionAdapterBuilder::prependEnterCall(SgScopeStatement *scope) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	SgExprStatement *functionCall = createEntryFunctionExpressionStatement(scope);

	if (functionCall == NULL) {
		throw InstroException("No functionCall statement was returned, when trying to instrument function");
	}

	SageInterface::prependStatement(functionCall, scope);
#ifdef DEBUG
	DEBUGEXIT
#endif
}

void GenericFunctionAdapterBuilder::appendExitCall(SgScopeStatement *scope) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	// We only want to append the exit-call if we do not have a return-statement at the end
	Rose_STL_Container<SgStatement *> scopeStatements = scope->getStatementList();
	Rose_STL_Container<SgStatement *>::iterator lastStatement = scopeStatements.end() - 1;

	if (scopeStatements.empty() || isSgReturnStmt(*lastStatement) == NULL) {
		SgExprStatement *functionCall = createExitFunctionExpressionStatement(scope);
		if (functionCall == NULL) {
			throw InstroException("No functionCall statement was returned, when trying to instrument function");
		}
		SageInterface::appendStatement(functionCall, scope);
	}
#ifdef DEBUG
	DEBUGEXIT
#endif
}

void GenericFunctionAdapterBuilder::assertUnparse(SgNode *node) {
	class AssertUnparse : public AstSimpleProcessing {
		void visit(SgNode *node) {
			if (isSgLocatedNode(node)) {
				node->set_isModified(true);
				if (node->get_file_info()) {
					node->get_file_info()->setOutputInCodeGeneration();
				}
			}
		}
	} assertUnparse;

	// Assert the unparsing of all parents
	SgNode *parent = node->get_parent();
	while (parent != 0) {
		if (isSgLocatedNode(parent)) {
			node->set_isModified(true);
			if (node->get_file_info()) {
				node->get_file_info()->setOutputInCodeGeneration();
			}
		}
		parent = parent->get_parent();
	}

	// Assert the unparsing of the node itself and all its children
	assertUnparse.traverse(node, preorder);
}

void GenericFunctionAdapterBuilder::declareEntryFunction(SgGlobal *globalScope) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	if (!entryFunctionHasBeenDeclared) {
		entryFunctionHasBeenDeclared = true;

		globalScope->prepend_statement(InstROBuilder::buildProfileFuncDeclarationEnter(globalScope));
	}
#ifdef DEBUG
	DEBUGEXIT
#endif
}

void GenericFunctionAdapterBuilder::declareExitFunction(SgGlobal *globalScope) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	if (!exitFunctionHasBeenDeclared) {
		exitFunctionHasBeenDeclared = true;

		globalScope->prepend_statement(InstROBuilder::buildProfileFuncDeclarationExit(globalScope));
		//		InstROBuilder::buildProfileFuncDeclarationExit(SageInterface::getGlobalScope(functionCall));
	}
#ifdef DEBUG
	DEBUGEXIT
#endif
}

void GenericFunctionAdapterBuilder::declareProfileFunction(SgFunctionDeclaration *profFunctionDecl,
																													 SgGlobal *globalScope) {
#ifdef DEBUG
	DEBUGENTRY
#endif
	// globalScope->prepend_statement(SageBuilder::buildDeclarationStatement(profFunctionDecl));
	globalScope->prepend_statement(profFunctionDecl);
#ifdef DEBUG
	DEBUGEXIT
#endif
}
