#include <instro/contrib/geimeradapter.h>

using namespace InstRO;

GeimerAdapterBuilder::GeimerAdapterBuilder(Selector* selectorToRespect)
		: AdapterBuilder(selectorToRespect), specProj(NULL) {
	this->selectorToRespect = selectorToRespect;
	this->includeHasBeenProcessed = false;
}
/* JPL: No need for this constructor in my opinion.
GeimerAdapterBuilder::GeimerAdapterBuilder(SgProject* specAst, Selector* selectorToRespect){
		this->specProj = specAst;
		this->selectorToRespect = selectorToRespect;
}
*/
void GeimerAdapterBuilder::buildForNode(SgNode* n, Selector* selectorToRespect) {
	// At each node we want a clean starting point
	this->declNodes.clear();
	this->entryNodes.clear();
	this->exitNodes.clear();
	this->abortNodes.clear();
	this->includeNodes.clear();
	this->initNodes.clear();
	this->mainInitNodes.clear();
	// The code to insert is stored in the ast-marker
	// --- retrieve marker
	// As only nodes which hold a certain selection marker should be processed here
	ASTAttributeContainer* container = ASTAttributeContainer::getASTAttributeContainer(n);
	if (!container->hasSelAttrib(this->selectorToRespect->getId())) {
		std::cerr << "Warning: Something went wrong. Node has no GeimerSelector Marker" << std::endl;
		return;
	}
	ASTAttributeContainer::SharedAttributePointerType sMarker = container->getSelAttrib(this->selectorToRespect->getId());
	// This is using the c++11/boost shared pointers.
	std::tr1::shared_ptr<GeimerSelectionASTMarker> gMarker =
			std::tr1::dynamic_pointer_cast<GeimerSelectionASTMarker>(sMarker);
	/*    {// --- Debug
					std::cout << "At Node: " << n << " : " << n->unparseToString() << std::endl;
			}
	*/
	// calculate what to do
	extractConstructsFromMarker(gMarker);

	// --- delegate to methods.
	populateIncludeConstructs(n);

	populateEntryConstructs(n);

	populateInitConstructs(n);

	populateAbortConstructs(n);

	populateExitConstructs(n);

	populateMainInitConstructs(n);

	populateVariableDeclarations(n);

	AstTests::runAllTests(SageInterface::getProject());
}

void GeimerAdapterBuilder::buildForNode(SgNode* n) {
	if (isNodeSelected(n)) {
		// At each node we want a clean starting point
		this->declNodes.clear();
		this->entryNodes.clear();
		this->exitNodes.clear();
		this->abortNodes.clear();
		this->includeNodes.clear();
		this->initNodes.clear();
		this->mainInitNodes.clear();
		// The code to insert is stored in the ast-marker
		// --- retrieve marker
		// As only nodes which hold a certain selection marker should be processed here
		ASTAttributeContainer* container = ASTAttributeContainer::getASTAttributeContainer(n);

		ASTAttributeContainer::SharedAttributePointerType sMarker =
				container->getSelAttrib(this->selectorToRespect->getId());
		// This is using the c++11/boost shared pointers.
		std::tr1::shared_ptr<GeimerSelectionASTMarker> gMarker =
				std::tr1::dynamic_pointer_cast<GeimerSelectionASTMarker>(sMarker);
		/*    {// --- Debug
						std::cout << "At Node: " << n << " : " << n->unparseToString() << std::endl;
				}
		*/
		// calculate what to do
		extractConstructsFromMarker(gMarker);

		// --- delegate to methods.
		populateIncludeConstructs(n);

		populateEntryConstructs(n);

		populateInitConstructs(n);

		populateAbortConstructs(n);

		populateExitConstructs(n);

		populateMainInitConstructs(n);

		populateVariableDeclarations(n);

		AstTests::runAllTests(SageInterface::getProject());
	}
}

void GeimerAdapterBuilder::populateVariableDeclarations(SgNode* n) {
	SgScopeStatement* scope = isSgScopeStatement(n);

	if (isSgFunctionDefinition(scope)) {
		SgBasicBlock* bb = isSgFunctionDefinition(scope)->get_body();
		// in order to always insert preceeding the first statement, we use a reverse_iterator
		std::vector<SgNode*>::reverse_iterator i;
		for (i = this->declNodes.rbegin(); i != this->declNodes.rend(); i++) {
			SgDeclarationStatement* decl = isSgDeclarationStatement(*i);
			ROSE_ASSERT(decl != NULL);

			// JPL: Check the symbol table before copying sub asts!!
			SgVariableDeclaration* vDecl = isSgVariableDeclaration(decl);
			ROSE_ASSERT(vDecl != NULL);
			SgVariableDefinition* vDef = vDecl->get_definition();
			if (bb->symbol_exists(vDef->get_vardefn()->get_name())) {
				std::cerr << "Warning: Symbol " << vDef->get_vardefn()->get_name() << " already exists in scope "
									<< scope->unparseToString() << ". Skipping." << std::endl;
				return;
			}

			// copy sub ast
			SgTreeCopy tCopy = SgTreeCopy();
			if (decl) {
				decl = isSgDeclarationStatement(decl->copy(tCopy));
			}
			// adjust parents
			decl->set_parent(bb);
			if (decl->hasExplicitScope()) {
				decl->set_scope(bb);
			}
			// set preceeding all other statements
			SageInterface::prependStatement(decl, bb);
			decl->setOutputInCodeGeneration();
		}
	} else {
		std::cerr << "Warning: The node to insert a variable declaration was no function definition" << std::endl;
	}
}

void GeimerAdapterBuilder::populateEntryConstructs(SgNode* n) {
	SgScopeStatement* scope = isSgScopeStatement(n);

	if (isSgFunctionDefinition(scope)) {
		SgBasicBlock* bb = isSgFunctionDefinition(scope)->get_body();
		// in order to always insert preceeding the first statement, we use a reverse_iterator
		std::vector<SgNode*>::reverse_iterator i;
		for (i = this->entryNodes.rbegin(); i != this->entryNodes.rend(); i++) {
			if (isSgDeclarationStatement((*i))) {
				std::cerr << "Error: Please use the DECL construct in the configuration file for declarations." << std::endl;
				continue;
			}
			// copy sub ast
			SgTreeCopy tCopy = SgTreeCopy();
			SgStatement* st = isSgStatement(*i);
			if (st) {
				st = isSgStatement(st->copy(tCopy));
			}
			// adjust parents
			st->set_parent(bb);
			if (st->hasExplicitScope()) {
				st->set_scope(bb);
			}
			// set preceeding all other statements
			SageInterface::prependStatement(st, bb);
			st->setOutputInCodeGeneration();
		}
	} else {
		std::cerr << "Warning: The node to insert a statement was no function definition" << std::endl;
	}
}

void GeimerAdapterBuilder::populateExitConstructs(SgNode* n) {
	// retrieve all return statements
	Rose_STL_Container<SgNode*> returnStatements = NodeQuery::querySubTree(n, V_SgReturnStmt);
	// maybe transform return statements
	Rose_STL_Container<SgNode*>::iterator i;
	for (i = returnStatements.begin(); i != returnStatements.end(); i++) {
		transformReturn(isSgReturnStmt(*i));
	}
	// insert code
	std::vector<SgNode*>::reverse_iterator rI;
	for (i = returnStatements.begin(); i != returnStatements.end(); i++) {
		for (rI = this->exitNodes.rbegin(); rI != this->exitNodes.rend(); rI++) {
			SgTreeCopy tCopy = SgTreeCopy();
			SgStatement* st = isSgStatement((*rI));
			if (st) {
				st = isSgStatement(st->copy(tCopy));
			}
			st->set_parent(SageInterface::getScope((*i)));
			if (st->hasExplicitScope()) {
				st->set_scope(SageInterface::getScope((*i)));
			}
			SageInterface::insertStatement(isSgStatement(*i), st);
			st->setOutputInCodeGeneration();
		}
	}
	// return
}

void GeimerAdapterBuilder::transformReturn(SgReturnStmt* returnStatement) {
	//    DEBUGENTRY
	bool returnsSimpleValue =
			(isSgValueExp(returnStatement->get_expression()) != 0) || (isSgVarRefExp(returnStatement->get_expression()) != 0);
	/* We should also not try to move the value if the function returns a void, i.e. if we have a "return;" */
	bool returnsVoid = (returnStatement->get_expression() == 0);

	if (returnsSimpleValue == false && returnsVoid == false) {
		/* At first we need to store the return value in a temporary variable */
		/* We can use the definition to get to the declaration and thus to the return type of the function */
		SgScopeStatement* scope = NULL;
		SgNode* node = returnStatement;
		while (!(scope = isSgScopeStatement(node))) {
			// maybe print the tree uprising
			node = node->get_parent();
		}
		//            std::cout << "scope now is: " << scope->unparseToString() << std::endl;

		// SgVariableDeclaration* temporaryVariableDeclaration =
		// builder->buildTemporaryVariable(SageInterface::getScope(returnStatement), returnStatement);
		SgVariableDeclaration* temporaryVariableDeclaration = InstROBuilder::buildTemporaryVariable(scope, returnStatement);
		//       std::cout << "/*\n(a) Temporary ( " << temporaryVariableDeclaration << " ) in " <<
		//       temporaryVariableDeclaration->get_parent()<< "\n*/" << std::endl;
		SageInterface::insertStatementBefore(returnStatement, temporaryVariableDeclaration);
		SgBasicBlock* bb = NULL;
		node = temporaryVariableDeclaration->get_parent();
		while (!(bb = isSgBasicBlock(node))) {
			node = node->get_parent();
		}
		/* Build the statement referencing the variable */
		SgVarRefExp* variableReference = SageBuilder::buildVarRefExp(temporaryVariableDeclaration);
		returnStatement->set_expression(variableReference);
	}
}

void GeimerAdapterBuilder::populateAbortConstructs(SgNode* n) {
	if (isSgFunctionDefinition(n) == NULL) {
		return;
	}
	// find all calls to functions
	Rose_STL_Container<SgNode*> functionCalls = NodeQuery::querySubTree(n, V_SgFunctionRefExp);
	SgFunctionDefinition* enclosingFuncDef = isSgFunctionDefinition(n);

	Rose_STL_Container<SgNode*>::iterator fIter;
	for (fIter = functionCalls.begin(); fIter != functionCalls.end(); fIter++) {
		SgFunctionRefExp* fRefExp = NULL;
		if ((fRefExp = isSgFunctionRefExp((*fIter))) != NULL) {
			// Check for the name of the function called. Proceed iff the name is abort
			// XXX JPL: Is there some better way than another string comparison
			if (fRefExp->getAssociatedFunctionDeclaration()->get_qualified_name().getString().compare("::abort") == 0) {
				// std::cout << "populateAbortConstructs found: " <<
				// fRefExp->getAssociatedFunctionDeclaration()->get_qualified_name() << std::endl;

				std::vector<SgNode*>::reverse_iterator i;
				for (i = this->abortNodes.rbegin(); i != this->abortNodes.rend(); i++) {
					SgTreeCopy tCopy = SgTreeCopy();
					SgStatement* st = isSgStatement(*i);
					if (st) {
						st = isSgStatement(st->copy(tCopy));
					}

					st->set_parent(enclosingFuncDef->get_body());
					if (st->hasExplicitScope()) {
						st->set_scope(enclosingFuncDef->get_body());
					}
					// We need the enclosing statement
					SgExprStatement* fRefStatement = isSgExprStatement(SageInterface::getEnclosingStatement(fRefExp));
					// to use the SageInterface for insertion
					SageInterface::insertStatement(fRefStatement, st, true);
					st->setOutputInCodeGeneration();
				}
			}
		}
	}
}

void GeimerAdapterBuilder::populateInitConstructs(SgNode* n) {
	// Our init construct is basically equal to the entry construct
}

void GeimerAdapterBuilder::populateMainInitConstructs(SgNode* n) {
	// This is what geimers init construct is like
	SgFunctionDeclaration* mainDecl = SageInterface::findMain(SageInterface::getProject());

	SgFunctionDefinition* mainDef = mainDecl->get_definition();
	SgBasicBlock* bb = mainDef->get_body();

	std::vector<SgNode*>::reverse_iterator i;
	for (i = this->mainInitNodes.rbegin(); i != this->mainInitNodes.rend(); i++) {
		// copy sub ast
		SgTreeCopy tCopy = SgTreeCopy();
		SgStatement* st = isSgStatement(*i);
		if (st) {
			st = isSgStatement(st->copy(tCopy));
		}
		// adjust parents
		st->set_parent(bb);
		if (st->hasExplicitScope()) {
			st->set_scope(bb);
		}
		// set preceeding all other statements
		SageInterface::prependStatement(st, bb);
		st->setOutputInCodeGeneration();
	}
}

void GeimerAdapterBuilder::populateIncludeConstructs(SgNode* n) {
	// we can include files. This has to be called only ONCE - not on every node.
	// Check if include statements have been processed.
	if (this->includeHasBeenProcessed) {
		return;
	}

	// In case include has not been processed, process
	SgGlobal* gScope = SageInterface::getFirstGlobalScope(SageInterface::getProject());
	if (ASTAttributeContainer::hasASTAttributeContainer(n)) {
		if (ASTAttributeContainer::getASTAttributeContainer(n)->hasSelAttrib(this->selectorToRespect->getId())) {
			//            std::cout << "Global Scope has selector attrib" << std::endl;
			std::tr1::shared_ptr<GeimerSelectionASTMarker> p_geimerAttrib =
					std::tr1::dynamic_pointer_cast<GeimerSelectionASTMarker>(
							ASTAttributeContainer::getASTAttributeContainer(n)->getSelAttrib(this->selectorToRespect->getId()));
			std::vector<std::string> vIncludes = p_geimerAttrib->getIncludeFilenames();
			//            std::cout << "The include vector has size: " << vIncludes.size() << std::endl;
			std::vector<std::string>::iterator i;
			for (i = vIncludes.begin(); i != vIncludes.end(); i++) {
				SageInterface::insertHeader((*i), PreprocessingInfo::after, true, gScope);
			}
		}
	}
}

void GeimerAdapterBuilder::extractConstructsFromMarker(std::tr1::shared_ptr<GeimerSelectionASTMarker> marker) {
	// Get the list of all matching SgNodes calculated from the selector
	std::vector<SgNode*> specAstNodes = marker->getSpecNodes();
	std::vector<SgNode*>::iterator i;

	// Traverse each SgNode (sub-AST)to find pragma declarations and extract sub-ASTs between
	for (i = specAstNodes.begin(); i != specAstNodes.end(); i++) {
		// --- we have to find pragma declarations
		SgNode* t_node = isSgNode((*i));
		std::vector<SgNode*> pragmas = NodeQuery::querySubTree(t_node, V_SgPragmaDeclaration);
		std::vector<SgNode*>::iterator pIter;

		// --- traverse the ast between those declarations
		for (pIter = pragmas.begin(); pIter != pragmas.end(); pIter++) {
			// get the child idx of the pragma declaration
			SgPragmaDeclaration* t_pragma = isSgPragmaDeclaration((*pIter));
			size_t pIdx = t_pragma->get_parent()->get_childIndex(t_pragma);
			// how many successor does our parent have?
			size_t nSuccessor = (*pIter)->get_parent()->get_numberOfTraversalSuccessors();
			/*
									{ // --- DEBUG
											std::cout << "Pragma declaration: " << t_pragma << " : name: " <<
				 t_pragma->get_pragma()->get_name() << " : " << t_pragma->unparseToString() << std::endl;
									}
			*/
			// Determine the type of pragma
			std::string pName = t_pragma->get_pragma()->get_name();
			int pos = pName.rfind(" ");
			std::string pType = pName.substr(pos + 1);

			{	// --- DEBUG
				 //                std::cout << "Type of Pragma: " << pType << std::endl;
			}	// ---

			// get the successing nodes until the next pragma declaration
			size_t idxOffset = 1;
			SgNode* tNode = t_pragma->get_parent()->get_traversalSuccessorByIndex(pIdx + idxOffset);
			while ((isSgPragmaDeclaration(tNode) == NULL) && ((idxOffset + pIdx < nSuccessor))) {
				// --- DEBUG
				{
					//                    std::cout << "nSuccessor: " << nSuccessor << " pIdx:" << pIdx << " idxOffset: " <<
					//                    idxOffset << "\nNode between Pragma: " << tNode->unparseToString() << std::endl;
				}
				// ---

				// Dependent on the type of the pragma we insert in our data containers
				insertIntoDataContainer(tNode, pType);

				// Get next Ast node
				idxOffset++;
				if (idxOffset + pIdx < nSuccessor)
					tNode = t_pragma->get_parent()->get_traversalSuccessorByIndex(pIdx + idxOffset);
			}
		}
	}
}

void GeimerAdapterBuilder::insertIntoDataContainer(SgNode* tNode, std::string pType) {
	std::vector<SgNode*>::iterator i;
	if (pType.compare("VARDECL") == 0) {
		for (i = this->declNodes.begin(); i != this->declNodes.end(); i++) {
			if (tNode->unparseToString().compare((*i)->unparseToString()) == 0) {
				std::cerr << "There have been two equal \"variable declarations\" for insertion." << std::endl;
				std::cerr << "The declaration was: " << (*i)->unparseToString() << std::endl;
				return;
			}
		}
		this->declNodes.push_back(tNode);
	} else if (pType.compare("ENTRY") == 0) {
		for (i = this->entryNodes.begin(); i != this->entryNodes.end(); i++) {
			if (tNode->unparseToString().compare((*i)->unparseToString()) == 0) {
				std::cerr << "There have been two equal \"entry\" code constructs for insertion." << std::endl;
				return;
			}
		}
		this->entryNodes.push_back(tNode);
	} else if (pType.compare("EXIT") == 0) {
		for (i = this->exitNodes.begin(); i != this->exitNodes.end(); i++) {
			if (tNode->unparseToString().compare((*i)->unparseToString()) == 0) {
				std::cerr << "There have been two equal \"exit\" code constructs for insertion." << std::endl;
				return;
			}
		}
		this->exitNodes.push_back(tNode);
	} else if (pType.compare("INIT") == 0) {
		this->initNodes.push_back(tNode);
	} else if (pType.compare("ABORT") == 0) {
		this->abortNodes.push_back(tNode);
	} else if (pType.compare("MAIN_INITIALIZER") == 0) {
		std::cout << "Maininitializer node found." << std::endl;
		this->mainInitNodes.push_back(tNode);
	} else if (pType.compare("INCL") == 0) {
		for (i = this->includeNodes.begin(); i != this->includeNodes.end(); i++) {
			if (tNode->unparseToString().compare((*i)->unparseToString()) == 0) {
				std::cerr << "There have been two equal \"include\" code constructs for insertion." << std::endl;
				return;
			}
		}
		//        std::cout << "Include pragma found and inserted into data-container. " << std::endl;
		this->includeNodes.push_back(tNode);
	}
}
