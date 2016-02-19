#include "instro/rose/pass/adapter/support/RoseCodeWrapper.h"

#include "instro/rose/utility/ASTHelper.h"
#include "instro/utility/Logger.h"

namespace InstRO {
namespace Rose {
namespace Adapter {
namespace Support {

void RoseCodeWrapper::wrapStatement(SgStatement* stmt, std::string postfix, size_t id) {
	if (insertHeaderIfSource(stmt)) {
		auto functionScope = SageInterface::getScope(stmt);
		SageInterface::insertStatementBefore(
				stmt, buildCallExpressionStatement(functionScope, std::string("__instro_start_") + postfix, id));

		auto endFuncCall = buildCallExpressionStatement(functionScope, std::string("__instro_end_") + postfix, id);

		// returns
		auto returnStmts = SageInterface::querySubTree<SgReturnStmt>(stmt, V_SgReturnStmt);
		for (auto returnStmt : returnStmts) {
			instrumentReturnStmt(SageInterface::getEnclosingScope(returnStmt), returnStmt, endFuncCall);
		}

		SageInterface::insertStatementAfter(stmt, endFuncCall);
	}
}

void RoseCodeWrapper::wrapExpression(SgExpression* expr, size_t id) {
	if (insertHeaderIfSource(expr)) {
	}
	// TODO implement me. Wrap expression in a function call. Instrument function.
	logIt(ERROR) << "RoseCodeWrapper: wrapExpression() not implemented." << std::endl;
}

void RoseCodeWrapper::instrumentFunction(SgFunctionDefinition* function, size_t id) {
	if (insertHeaderIfSource(function)) {
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
	if (insertHeaderIfSource(scope)) {
		// start
		SageInterface::prependStatement(buildCallExpressionStatement(scope, "__instro_start_scope", id), scope);
		// continues and breaks
		auto endFuncCall = buildCallExpressionStatement(scope, "__instro_end_scope", id);
		auto potentialExits = SageInterface::querySubTree<SgStatement>(scope, V_SgBreakStmt);
		auto continueStmts = SageInterface::querySubTree<SgStatement>(scope, V_SgContinueStmt);
		potentialExits.insert(potentialExits.end(), continueStmts.begin(), continueStmts.end());
		for (auto exit : potentialExits) {
			instrumentPossibleExit(scope, exit, endFuncCall);
		}
		// returns
		auto returnStmts = SageInterface::querySubTree<SgReturnStmt>(scope, V_SgReturnStmt);
		for (auto returnStmt : returnStmts) {
			instrumentReturnStmt(scope, returnStmt, endFuncCall);
		}
		// scope end
		SageInterface::appendStatement(endFuncCall, scope);
	}
}

void RoseCodeWrapper::instrumentPossibleExit(SgScopeStatement* scope, SgStatement* exit, SgStatement* instrumentStmt) {
	SgStatement* scopeEnclosingLoop = InstRO::Rose::Utility::ASTHelper::getEnclosingLoop(scope);
	SgStatement* exitEnclosingLoop = InstRO::Rose::Utility::ASTHelper::getEnclosingLoop(exit);

	if (scopeEnclosingLoop == exitEnclosingLoop) {
		SageInterface::insertStatementBefore(exit, SageInterface::copyStatement(instrumentStmt));
	}
}

void RoseCodeWrapper::instrumentReturnStmt(SgScopeStatement* scope, SgReturnStmt* returnStmt,
																					 SgStatement* instrumentStmt) {
	auto returnExpr = returnStmt->get_expression();
	if (returnExpr != nullptr && !(isSgValueExp(returnExpr)) && !(isSgVarRefExp(returnExpr)) && !(isSgNullExpression(returnExpr))) {
		SageInterface::splitExpression(returnExpr);	// return statement transformation
	}

	SageInterface::insertStatementBefore(returnStmt, SageInterface::copyStatement(instrumentStmt));
}

bool RoseCodeWrapper::insertHeaderIfSource(SgLocatedNode* node) {
	SgSourceFile* sourceFile = isSgSourceFile(SageInterface::getEnclosingFileNode(node));
	std::string fileInfoName = node->get_file_info()->get_filenameString();
	std::string sourceFileName = sourceFile->getFileName();

	// if the two names do not match, the construct originates from an include
	if (sourceFile != nullptr && (fileInfoName == sourceFileName)) {
		if (filesWithInclude.find(sourceFile) == filesWithInclude.end()) {
			SageInterface::insertHeader("InstROMeasurementInterface.h", PreprocessingInfo::before, true,
																	sourceFile->get_globalScope());
			filesWithInclude.insert(sourceFile);
			logIt(INFO) << "RoseCodeWrapper: inserted header in " << sourceFileName << std::endl;
		}
		return true;
	}
	logIt(WARN) << "RoseCodeWrapper: header instrumentation is not supported. " << fileInfoName << std::endl;
	return false;
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
