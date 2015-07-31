#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/instrobuilder.h"

namespace InstRO {
	namespace Rose {
		namespace Adapter {
			namespace AdapterSupport{

				/**
				 * Creates the cyg_profile_func_enter with an empty function parameter list.
				 */
				SgDeclarationStatement* InstROBuilder::buildFunctionDeclaration(SgScopeStatement* scope, std::string functionName) {
					// Parameterlist is provided by buildDeclarationParameterList()
					SgInitializedName* this_function_name_enter = SageBuilder::buildInitializedName("this_fn", SageBuilder::buildPointerType(SageBuilder::buildVoidType()));
					SgInitializedName* call_site_name_enter = SageBuilder::buildInitializedName("call_site", SageBuilder::buildPointerType(SageBuilder::buildVoidType()));
					SgFunctionParameterList* functionParameterList = SageBuilder::buildFunctionParameterList(this_function_name_enter, call_site_name_enter);

					SgFunctionDeclaration* enterFunction = SageBuilder::buildNondefiningFunctionDeclaration(functionName, SageBuilder::buildVoidType(), functionParameterList, scope);
					// Prepend "extern 'C'" in front of the function-declaration
					if (SageInterface::getEnclosingNode<SgProject>(scope)->get_Cxx_only()) {
						isSgDeclarationStatement(enterFunction)->set_linkage("C");
					}
					SageInterface::setExtern(enterFunction);

					return enterFunction;
				}

				SgExprStatement* InstROBuilder::buildFunctionCall(SgScopeStatement* scope, SgExpression* firstArgument, SgExpression* secondArgument, std::string functionName) {
					/* Get the argument list for the given function */
					SgExprListExp* argument_expression_list_enter = buildFunctionCallArgumentList(firstArgument, secondArgument);

					return SageBuilder::buildFunctionCallStmt(functionName, SageBuilder::buildVoidType(), argument_expression_list_enter, SageInterface::getGlobalScope(scope));
				}

				SgExprListExp* InstROBuilder::buildFunctionCallArgumentList(SgExpression* firstArgument, SgExpression* secondArgument) {
					/* We cast both given arguments to void pointers if they are not of that type already */
					/* Check the first argument and cast if necessary */
					SgExpression* firstArgumentCast = 0;
					if (isSgCastExp(firstArgument) && isSgPointerType(isSgCastExp(firstArgument)->get_type()) && isSgTypeVoid(isSgPointerType(isSgCastExp(firstArgument)->get_type())->get_base_type())) {
						firstArgumentCast = firstArgument;
					}
					else {
						firstArgumentCast = SageBuilder::buildCastExp(firstArgument, SageBuilder::buildPointerType(SageBuilder::buildVoidType()));
					}

					/* Check the second argument and cast if necessary */
					SgExpression* secondArgumentCast = 0;
					if (isSgCastExp(secondArgument) && isSgPointerType(isSgCastExp(secondArgument)->get_type()) && isSgTypeVoid(isSgPointerType(isSgCastExp(secondArgument)->get_type())->get_base_type())) {
						secondArgumentCast = secondArgument;
					}
					else {
						secondArgumentCast = SageBuilder::buildCastExp(secondArgument, SageBuilder::buildPointerType(SageBuilder::buildVoidType()));
					}

					/* Build the argument list with both arguments properly cast */
					return SageBuilder::buildExprListExp(firstArgumentCast, secondArgumentCast);
				}

				SgExprStatement* InstROBuilder::buildCallToPushIdentifierToStackFunction(size_t identifier, SgExpression* threadIdentifierExpr, SgScopeStatement* scope) {
					// The function expression
					SgExpression* identifierExpr = SageBuilder::buildUnsignedLongLongIntVal(identifier);

					// Create call to pthread.h pthread_self()
					SgExprListExp* argumentListExpression = SageBuilder::buildExprListExp(identifierExpr, threadIdentifierExpr);

					// Build the call itself
					return SageBuilder::buildFunctionCallStmt("_instroPushIdentifier", SageBuilder::buildVoidType(), argumentListExpression, scope);
				}

				SgExprStatement* InstROBuilder::buildCallToPopIdentifierFromStackFunction(SgExpression* threadIdentifierExp, SgScopeStatement* scope) {
					// Build Sage expressions

					// Create call to pthread.h pthread_self()
					//SgExprListExp* argumentListExpression = SageBuilder::buildExprListExp(pthreadSelfCallExp);
					SgExprListExp* argumentListExpression = SageBuilder::buildExprListExp(threadIdentifierExp);

					// Build the call itself
					return SageBuilder::buildFunctionCallStmt("_instroPopIdentifier", SageBuilder::buildVoidType(), argumentListExpression, scope);
				}

				SgFunctionDeclaration* InstROBuilder::buildPushIdentifierToStackDeclaration(SgScopeStatement* scope) {
					SgInitializedName* funcIdentifier = SageBuilder::buildInitializedName("funcIdentifier", SageBuilder::buildUnsignedLongLongType());
					SgInitializedName* threadIdentifier = SageBuilder::buildInitializedName("threadIdentifier", SageBuilder::buildUnsignedLongLongType());
					SgFunctionParameterList* paramList = SageBuilder::buildFunctionParameterList(funcIdentifier, threadIdentifier);

					SgFunctionDeclaration* pushFunctionDecl = SageBuilder::buildNondefiningFunctionDeclaration("_instroPushIdentifier", SageBuilder::buildVoidType(), paramList, SageInterface::getGlobalScope(scope));
					if (SageInterface::getEnclosingNode<SgProject>(scope)->get_Cxx_only()) {
						pushFunctionDecl->set_linkage("C");
					}
					SageInterface::setExtern(pushFunctionDecl);

					return pushFunctionDecl;
				}
				SgFunctionDeclaration* InstROBuilder::buildPopIdentifierFromStackDeclaration(SgScopeStatement* scope) {
					SgInitializedName* threadIdentifier = SageBuilder::buildInitializedName("threadIdentifier", SageBuilder::buildUnsignedLongLongType());
					SgFunctionParameterList* paramList = SageBuilder::buildFunctionParameterList(threadIdentifier);

					SgFunctionDeclaration* popFunctionDecl = SageBuilder::buildNondefiningFunctionDeclaration("_instroPopIdentifier", SageBuilder::buildVoidType(), paramList, SageInterface::getGlobalScope(scope));
					if (SageInterface::getEnclosingNode<SgProject>(scope)->get_Cxx_only()) {
						popFunctionDecl->set_linkage("C");
					}
					SageInterface::setExtern(popFunctionDecl);

					return popFunctionDecl;
				}

				/** \} */

				/** \name Build templates */
				/** \{ */
				SgTemplateInstantiationFunctionDecl* InstROBuilder::buildForwardDeclaration(SgTemplateInstantiationFunctionDecl* originalDeclaration) {
					std::cerr << "INSTRO ERROR: CALL TO OUTDATED FUNCTION...." << std::endl;
					return NULL;
				}

				SgTemplateInstantiationMemberFunctionDecl* InstROBuilder::buildForwardDeclaration(SgTemplateInstantiationMemberFunctionDecl* originalDeclaration) {
					SgTemplateInstantiationMemberFunctionDecl* forwardDeclaration = NULL;
					if (originalDeclaration == originalDeclaration->get_definingDeclaration()) {
						SgFunctionDefinition* def = originalDeclaration->get_definition();
						originalDeclaration->set_definition(NULL);
						forwardDeclaration = SageInterface::buildForwardFunctionDeclaration(originalDeclaration);
						originalDeclaration->set_definition(def);
					}
					else {
						forwardDeclaration = SageInterface::buildForwardFunctionDeclaration(originalDeclaration);
					}

					forwardDeclaration->setOutputInCodeGeneration();
					return forwardDeclaration;
				}

				SgTemplateInstantiationFunctionDecl* InstROBuilder::buildForwardDeclaration(SgTemplateInstantiationFunctionDecl* originalDeclaration, SgScopeStatement* futureScope) {
					// We build the copied declaration as above, but here we know the future scope.
					SgTemplateInstantiationFunctionDecl* copyDeclaration = 0;
					ROSE_ASSERT(futureScope != NULL);

					// Get the function name without the template things and build a SgName
					std::string functionNameWithoutTemplateArguments = originalDeclaration->get_name().getString().substr(0, originalDeclaration->get_name().getString().find("<"));
					SgName functionNameWithoutTemplateArgumentsName = SgName(functionNameWithoutTemplateArguments);

					// Use new Rose SageBuilder ability to create template instantiations
					copyDeclaration = isSgTemplateInstantiationFunctionDecl(SageBuilder::buildNondefiningFunctionDeclaration(functionNameWithoutTemplateArgumentsName, originalDeclaration->get_type(), originalDeclaration->get_parameterList(), futureScope, originalDeclaration->get_decoratorList(), true, &(originalDeclaration->get_templateArguments())));
					copyDeclaration->set_type(originalDeclaration->get_type());
					return copyDeclaration;
				}

				//		static SgTemplateInstantiationMemberFunctionDecl* buildForwardDeclaration(SgTemplateInstantiationMemberFunctionDecl* originalDeclaration, SgScopeStatement* futureScope);
				/** \} */

				/** \name Build unions */
				/** \{ */
				SgClassDeclaration* InstROBuilder::buildUnion(SgFunctionDefinition* functionDefinition, SgVariableDeclaration*& unionMemberfunctionPointer, SgVariableDeclaration*& unionVoidPointer) {
					std::cout << "INSTRO IMPLEMENTATION IN PROGRESS ERROR!" << std::endl;
					// TODO 2013-10-23 JP: Needs to be implemented

					// function is not a member function, nothing to do
					if (isSgMemberFunctionDeclaration(functionDefinition->get_declaration()) == NULL)
						return NULL;

					std::string unionName = "instrumentation_union";
					std::string unionPointernName = "instrumentation_union_void_pointer";

					std::cout << "Getting currentScope()" << std::endl;
					SgScopeStatement *currentScope = SgType::getCurrentScope();
					if (currentScope != NULL){
						std::cout << "Current Scope: " << currentScope << std::endl;
					}
					//	SageBuilder::pushScopeStack(functionDefinition->get_body());
					// declare a union declaration and definition
					SgClassDeclaration* unionDecl = SageBuilder::buildNondefiningClassDeclaration(unionName, functionDefinition->get_body());
					unionDecl->set_class_type(SgClassDeclaration::e_union);

					SgClassDefinition* unionDef = SageBuilder::buildClassDefinition(unionDecl);
					unionDecl->set_definition(unionDef);

					return NULL;
				}

				SgVariableDeclaration* InstROBuilder::buildUnionVariableDeclaration(SgScopeStatement* scope, SgClassDeclaration* unionDeclaration) {
					std::cout << "INSTRO NOT implemented YET ERROR!" << std::endl;
					// TODO 2013-10-23 JP: Needs to be implemented

					return NULL;
				}

				SgExprStatement* InstROBuilder::buildMemberfunctionPointerAssignment(SgScopeStatement* scope, SgScopeStatement* functionDefinition, SgClassDeclaration* unionDeclaration, SgVariableDeclaration* unionMemberfunctionPointer, SgVariableDeclaration* unionVariableDeclaration) {
					std::cout << "INSTRO NOT implemented YET ERROR!" << std::endl;
					// TODO 2013-10-23 JP: Needs to be implemented
					return NULL;
				}

				SgExpression* InstROBuilder::buildMemberVariableReferenceExpression(SgVariableDeclaration* unionVariableDeclaration, SgVariableDeclaration* memberVariableDeclaration) {
					std::cout << "INSTRO NOT implemented YET ERROR!" << std::endl;
					// TODO 2013-10-23 JP: Needs to be implemented
					return NULL;
				}
				/** \} */

				SgTryStmt* InstROBuilder::buildTryCatchStmt(SgStatement* content) {
					/* Build the ellipsis catching all variables */
					SgVariableDeclaration* condition = SageBuilder::buildVariableDeclaration(" ", new SgTypeEllipse(), 0, 0);

					/* Build the catch clause body */
					SgBasicBlock* catchBody = SageBuilder::buildBasicBlock();

					/* Build the catch clause */
					SgCatchOptionStmt* catchClause = SageBuilder::buildCatchOptionStmt(condition, catchBody);

					/* fix variable scope */
					SageInterface::fixVariableDeclaration(condition, catchClause);

					/* Build the try body */
					SgBasicBlock* tryBody = SageBuilder::buildBasicBlock();

					/* Build the try statement with catch clause */
					SgTryStmt* tryStmt = SageBuilder::buildTryStmt(tryBody, catchClause);

					/* FIX MZ: ROSE has a problem with try-catch as function body, so an additional body block is needed */
					if (isSgBasicBlock(content) && isSgFunctionDefinition(content->get_parent())) {
						SgBasicBlock* newBody = SageBuilder::buildBasicBlock();
						SageInterface::replaceStatement(content, newBody);
						SageInterface::prependStatement(tryStmt, newBody);
					}
					else {
						/* Replace content with try-catch statement */
						SageInterface::replaceStatement(content, tryStmt);
					}

					/* Insert content into try block */
					if (isSgBasicBlock(content)) {
						SageInterface::replaceStatement(tryBody, content);
					}
					else {
						SageInterface::prependStatement(content, tryBody);
					}

					return tryStmt;
				}

				SgExprStatement* InstROBuilder::buildRethrowStatement(void) {
					/* Since there are no builders present yet for catch-blocks, we have to take care of the AST ourselves */
					/* Build the rethrow-expression */
					SgThrowOp* throwOp = new SgThrowOp(0, SageBuilder::buildUnknownType(), SgThrowOp::rethrow);

					/* Build a statement out of it */
					SgExprStatement* retVal = SageBuilder::buildExprStatement(throwOp);

					/* Mark the rethrow-expression for unparsing */
					SageInterface::setSourcePosition(throwOp);

					return retVal;
				}
				/** \} */

				/**
				 * \brief Builds a pointer to the given function and casts it to void*
				 * \param functionDefinition The definition of the function a pointer is to be built to
				 * \return A void* pointing to the function
				 */
				SgExpression* InstROBuilder::buildFunctionPointerCastToVoid(SgFunctionDefinition* functionDefinition) {
					std::cout << "INSTRO NOT implemented YET ERROR!" << std::endl;
					// TODO 2013-10-23 JP: Needs to be implemented
					return NULL;
				}
				/**
				 * \brief Builds a simple null pointer
				 * XXX 2013-10-22 JP: This basically builds an int value of 0.
				 * \return A null pointer
				 */
				SgExpression* InstROBuilder::buildNullPointer(void) {
					return SageBuilder::buildIntVal(0);
				}

				/**
				 * \brief Builds a basic block containing the given statement
				 * \param content The content for the new block. If zero, an empty block is returned
				 * \return The newly built block
				 */
				SgBasicBlock* InstROBuilder::buildBlock(SgStatement* content) {
					if (content != 0) {
						return SageBuilder::buildBasicBlock(content);
					}
					else {
						return SageBuilder::buildBasicBlock();
					}
				}

				SgVariableDeclaration* InstROBuilder::buildTemporaryVariable(SgScopeStatement* scope, SgReturnStmt* returnStatement) {
					std::string tempReturnVarName = "instro_return_var_";
					std::stringstream ss;
					ss << std::hex << returnStatement;
					tempReturnVarName += ss.str();
					SgExpression* returnExpression = returnStatement->get_expression();
					/* CI 20120506: The following is faulty, as we need to get the return type of the function and not the type of the expression
					 // At first we need to get the return-type of the function to instrument
					 SgType* returnType = returnExpression->get_type();
					 */

					// XXX 2013-10-16 JP: Use the SageInterface here to get the next outer function definition.
					/* CI: travers the AST upwards to the function declaration in which the return statement is called. Use it to get the return type of the function and proceed as usual */
					SgNode * current = returnStatement;
					while (!isSgFunctionDefinition(current))
						current = current->get_parent();
					assert(isSgFunctionDefinition(current) != NULL);

					SgType* returnType = isSgFunctionDefinition(current)->get_declaration()->get_orig_return_type();
					assert(returnType != NULL);
					//	std::cout << "DEBUG: ReturnType during returnStatement transformation: " << returnType->unparseToString() << std::endl;
					/* Furthermore we want to declare a new variable and assign a value to it, so we build a initializer for it */
					SgAssignInitializer* initializer = SageBuilder::buildAssignInitializer(returnExpression, returnType);
					//    std::cout << "/*\n" << "initializer @ " << initializer << "\n*/" << std::endl;

					/* Having gathered all necessary information and constructs we finally build the defining variable declaration */
					SgVariableDeclaration* variableDeclaration = SageBuilder::buildVariableDeclaration(tempReturnVarName, returnType, initializer, scope);
					//    std::cout << "/*\n" << "var decl @ " << variableDeclaration << " : " <<variableDeclaration->unparseToString() << "\n*/" << std::endl;

					return variableDeclaration;
				}
			}
		}
	}
}