#ifndef INSTRO_ROSE_PASS_ADAPTER_STRATEGYBASEDADAPTER_INSTRO_BUILDER_H
#define INSTRO_ROSE_PASS_ADAPTER_STRATEGYBASEDADAPTER_INSTRO_BUILDER_H

#include "rose.h"

namespace InstRO {
	namespace Rose {
		namespace Adapter {
			namespace StrategyBasedAdapterSupport{

				/**
				 * \brief Provides building functions for several AST constructs
				 *
				 * This namespace provides another layer of abstraction and is used to build whole AST constructs,
				 * which can then be used inside ASTReWriters, to transform the AST.
				 *
				 * Code basically taken from Builder.cpp
				 *
				 * \todo 2013-10-24 JP: improve documentation!
				 */
				namespace InstROBuilder {
					/** \name Build profiling functions */
					/** \{ */
					SgDeclarationStatement* buildFunctionDeclaration(SgScopeStatement* scope, std::string functionName);
					SgExprStatement* buildFunctionCall(SgScopeStatement* scope, SgExpression* firstArgument, SgExpression* secondArgument, std::string functionName);
					SgExprListExp* buildFunctionCallArgumentList(SgExpression* firstArgument, SgExpression* secondArgument);
					/** \} */

					/** \name Build stack shadowing profiling functions */
					/** \{ */
					//SgExprStatement* buildCallToPushIdentifierToStackFunction(size_t identifier, size_t threadIdentifier, SgScopeStatement* scope);
					SgExprStatement* buildCallToPushIdentifierToStackFunction(size_t identifier, SgExpression* threadIdentifierExpr, SgScopeStatement* scope);
					//SgExprStatement* buildCallToPopIdentifierFromStackFunction(size_t threadIdentifier, SgScopeStatement* scope);
					SgExprStatement* buildCallToPopIdentifierFromStackFunction(SgExpression* threadIdentifierExp, SgScopeStatement* scope);
					SgFunctionDeclaration* buildPushIdentifierToStackDeclaration(SgScopeStatement* scope);
					SgFunctionDeclaration* buildPopIdentifierFromStackDeclaration(SgScopeStatement* scope);
					/** \} */

					/** \name Build templates */
					/** \{ */
					SgTemplateInstantiationFunctionDecl* buildForwardDeclaration(SgTemplateInstantiationFunctionDecl* originalDeclaration);
					SgTemplateInstantiationMemberFunctionDecl* buildForwardDeclaration(SgTemplateInstantiationMemberFunctionDecl* originalDeclaration);
					SgTemplateInstantiationFunctionDecl* buildForwardDeclaration(SgTemplateInstantiationFunctionDecl* originalDeclaration, SgScopeStatement* futureScope);

					//		static SgTemplateInstantiationMemberFunctionDecl* buildForwardDeclaration(SgTemplateInstantiationMemberFunctionDecl* originalDeclaration, SgScopeStatement* futureScope);
					/** \} */

					/** \name Build unions */
					/** \{ */
					SgClassDeclaration* buildUnion(SgFunctionDefinition* functionDefinition, SgVariableDeclaration*& unionMemberfunctionPointer, SgVariableDeclaration*& unionVoidPointer);
					SgVariableDeclaration* buildUnionVariableDeclaration(SgScopeStatement* scope, SgClassDeclaration* unionDeclaration);
					SgExprStatement* buildMemberfunctionPointerAssignment(SgScopeStatement* scope, SgScopeStatement* functionDefinition, SgClassDeclaration* unionDeclaration, SgVariableDeclaration* unionMemberfunctionPointer, SgVariableDeclaration* unionVariableDeclaration);
					SgExpression* buildMemberVariableReferenceExpression(SgVariableDeclaration* unionVariableDeclaration, SgVariableDeclaration* memberVariableDeclaration);
					/** \} */

					/** \name Build try-statements */
					/** \{ */
					SgTryStmt* buildTryCatchStmt(SgStatement* content);
					//SgCatchOptionStmt* buildCatchAll(SgStatement* body, SgTryStmt* tryStatement);
					SgExprStatement* buildRethrowStatement(void);
					/** \} */

					/**
					 * \brief Builds a pointer to the given function and casts it to void*
					 * \param functionDefinition The definition of the function a pointer is to be built to
					 * \return A void* pointing to the function
					 */
					SgExpression* buildFunctionPointerCastToVoid(SgFunctionDefinition* functionDefinition);
					/**
					 * \brief Builds a simple null pointer
					 * \return A null pointer
					 */
					SgExpression* buildNullPointer(void);

					/**
					 * \brief Builds a basic block containing the given statement
					 * \param content The content for the new block. If zero, an empty block is returned
					 * \return The newly built block
					 */
					SgBasicBlock* buildBlock(SgStatement* content = 0);

					SgVariableDeclaration* buildTemporaryVariable(SgScopeStatement* scope, SgReturnStmt* returnStatement);

				}
			}
		}
	}
}
#endif
