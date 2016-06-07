#include "instro/rose/pass/adapter/support/RoseCodeWrapper.h"

#include "instro/rose/utility/ASTHelper.h"
#include "instro/rose/utility/ASTTransformer.h"
#include "instro/Instrumentor.h"
#include "instro/utility/CommandlineHandling.h"

#include "instro/utility/Logger.h"

namespace InstRO {
namespace Rose {
namespace Adapter {
namespace Support {

void RoseCodeWrapper::wrapStatement(SgStatement* stmt, std::string postfix, size_t id) {
	auto& helper = Utility::ASTTransformation::HeaderIncludeHelper::getInstance();
	if (helper.insertHeaderIfSource("InstROMeasurementInterface.h", stmt)) {
		auto functionScope = SageInterface::getScope(stmt);
		SageInterface::insertStatementBefore(
				stmt, buildCallExpressionStatement(functionScope, std::string("__instro_start_") + postfix, id));

		auto endFuncCall = buildCallExpressionStatement(functionScope, std::string("__instro_end_") + postfix, id);
		// continues, breaks, returns
		if (isSgScopeStatement(stmt)) {
			handleRelevantExits(isSgScopeStatement(stmt), endFuncCall);
		}

		SageInterface::insertStatementAfter(stmt, endFuncCall);
	}
}

void RoseCodeWrapper::wrapExpression(SgExpression* expr, size_t id) {
	auto& helper = Utility::ASTTransformation::HeaderIncludeHelper::getInstance();
	if (helper.insertHeaderIfSource("InstROMeasurementInterface.h", expr)) {
	}
	// TODO implement me. Wrap expression in a function call. Instrument function.
	logIt(ERROR) << "RoseCodeWrapper: wrapExpression() not implemented." << std::endl;
}

void RoseCodeWrapper::instrumentFunction(SgFunctionDefinition* function, size_t id) {
	auto optionMap = Instrumentor::getCmdLineHandlerMap();
	// If the user did not manually specify that he wants to instrument ctors, we skip them
	auto optValue = optionMap.get<InstRO::Utility::CommandLineHandler::OptionArguments>("DefaultOpts");
	if (!optValue->instrumentCtors && Utility::ASTHelper::isConstructor(function)) {
		logIt(INFO) << "RoseCodeWrapper: Skipping constructor" << std::endl;
		return;
	}
	if (!optValue->instrumentDtors && Utility::ASTHelper::isDestructor(function)) {
		logIt(INFO) << "RoseCodeWrapper: Skipping destructor" << std::endl;
		return;
	}

	auto& helper = Utility::ASTTransformation::HeaderIncludeHelper::getInstance();
	if (helper.insertHeaderIfSource("InstROMeasurementInterface.h", function)) {
		// start
		auto voidPointer = SageBuilder::buildCastExp(SageBuilder::buildIntVal(0),
																								 SageBuilder::buildPointerType(SageBuilder::buildVoidType()));
		auto parameters = SageBuilder::buildExprListExp(SageBuilder::buildUnsignedLongLongIntVal(id), voidPointer);

		SageInterface::prependStatement(
				buildCallExpressionStatement(function->get_body(), "__instro_start_function", parameters),
				function->get_body());

		// end
		SageInterface::instrumentEndOfFunction(function->get_declaration(),
																					 buildCallExpressionStatement(function, "__instro_end_function", parameters));
	}
}

void RoseCodeWrapper::instrumentScope(SgScopeStatement* scope, size_t id) {
	auto& helper = Utility::ASTTransformation::HeaderIncludeHelper::getInstance();
	if (helper.insertHeaderIfSource("InstROMeasurementInterface.h", scope)) {
		// scope start
		SageInterface::prependStatement(buildCallExpressionStatement(scope, "__instro_start_scope", id), scope);

		auto endFuncCall = buildCallExpressionStatement(scope, "__instro_end_scope", id);
		// continues, breaks, returns
		handleRelevantExits(scope, endFuncCall);

		// scope end
		SageInterface::appendStatement(endFuncCall, scope);
	}
}

void RoseCodeWrapper::handleRelevantExits(SgScopeStatement* scope, SgStatement* instrumentStmt) {
	// breaks except those in inner loops or switches
	for (auto exit : SageInterface::findBreakStmts(scope)) {
		SageInterface::insertStatementBefore(exit, SageInterface::copyStatement(instrumentStmt));
	}
	// continues except those in inner loops
	for (auto exit : SageInterface::findContinueStmts(scope)) {
		SageInterface::insertStatementBefore(exit, SageInterface::copyStatement(instrumentStmt));
	}
	// all returns
	auto returnStmts = SageInterface::querySubTree<SgReturnStmt>(scope, V_SgReturnStmt);
	for (auto returnStmt : returnStmts) {
		instrumentReturnStmt(scope, returnStmt, instrumentStmt);
	}
}

void RoseCodeWrapper::instrumentReturnStmt(SgScopeStatement* scope, SgReturnStmt* returnStmt,
																					 SgStatement* instrumentStmt) {
	auto returnExpr = returnStmt->get_expression();
	if (returnExpr != nullptr && !(isSgValueExp(returnExpr)) && !(isSgVarRefExp(returnExpr)) &&
			!(isSgNullExpression(returnExpr))) {
		SageInterface::splitExpression(returnExpr);	// return statement transformation
	}

	SageInterface::insertStatementBefore(returnStmt, SageInterface::copyStatement(instrumentStmt));
}

SgStatement* RoseCodeWrapper::buildCallExpressionStatement(SgScopeStatement* context, std::string functionName,
																													 SgExprListExp* parameters) {
	SgExpression* call =
			SageBuilder::buildFunctionCallExp(functionName, SageBuilder::buildVoidType(), parameters, context);

	return SageBuilder::buildExprStatement(call);
}

SgStatement* RoseCodeWrapper::buildCallExpressionStatement(SgScopeStatement* context, std::string functionName,
																													 size_t id) {
	auto parameters = SageBuilder::buildExprListExp(SageBuilder::buildUnsignedLongLongIntVal(id));
	return buildCallExpressionStatement(context, functionName, parameters);
}

}	// namespace Support
}	// namespace Adapter
}	// namespace Rose
}	// namespace InstRO
