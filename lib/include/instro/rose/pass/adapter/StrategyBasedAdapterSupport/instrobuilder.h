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

					/** \name Build try-statements */
					/** \{ */
					SgTryStmt* buildTryCatchStmt(SgStatement* content);
					//SgCatchOptionStmt* buildCatchAll(SgStatement* body, SgTryStmt* tryStatement);
					SgExprStatement* buildRethrowStatement(void);
					/** \} */

					SgVariableDeclaration* buildTemporaryVariable(SgScopeStatement* scope, SgReturnStmt* returnStatement);

				}
			}
		}
	}
}
#endif
