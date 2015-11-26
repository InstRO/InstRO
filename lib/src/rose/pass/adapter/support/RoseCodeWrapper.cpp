#include "instro/rose/pass/adapter/support/RoseCodeWrapper.h"

#include "instro/utility/Logger.h"

/// XXX
#include <cassert>

namespace InstRO {
namespace Rose {
namespace Adapter {
namespace Support {

void RoseCodeWrapper::wrapStatement(SgStatement* stmt, std::string postfix, size_t id) {
	if (insertHeaderIfSource(stmt)) {
		auto functionScope = SageInterface::getScope(stmt);
		SageInterface::insertStatementBefore(
				stmt, buildCallExpressionStatement(functionScope, std::string("__instro_start_") + postfix, id));
		SageInterface::insertStatementAfter(
				stmt, buildCallExpressionStatement(functionScope, std::string("__instro_end_") + postfix, id));
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

		// START
		auto voidPointer = SageBuilder::buildCastExp(SageBuilder::buildIntVal(0),
																								 SageBuilder::buildPointerType(SageBuilder::buildVoidType()));
		auto parameters = SageBuilder::buildExprListExp(SageBuilder::buildUnsignedLongLongIntVal(id), voidPointer);

		SageInterface::prependStatement(
				buildCallExpressionStatement(function->get_body(), "__instro_start_function", parameters),
				function->get_body());

		// END
		SageInterface::instrumentEndOfFunction(
				function->get_declaration(),
				buildCallExpressionStatement(function, "__instro_end_function", parameters));
	}
}

void RoseCodeWrapper::instrumentScope(SgScopeStatement* scope, size_t id) {
	if (insertHeaderIfSource(scope)) {
		SageInterface::prependStatement(buildCallExpressionStatement(scope, "__instro_start_scope", id), scope);

		// TODO do not end after return statement
		// TODO other possible exits from a scope: continue, break, return, goto

		SageInterface::appendStatement(buildCallExpressionStatement(scope, "__instro_end_scope", id), scope);
	}
}

bool RoseCodeWrapper::insertHeaderIfSource(SgLocatedNode* node) {
	SgFile* file = SageInterface::getEnclosingFileNode(node);
	SgSourceFile* sourceFile = isSgSourceFile(file);

	if (sourceFile != nullptr) {
		if (filesWithInclude.find(sourceFile) == filesWithInclude.end()) {
			SageInterface::insertHeader("InstROMeasurementInterface.h", PreprocessingInfo::before, true,
																	sourceFile->get_globalScope());
			filesWithInclude.insert(sourceFile);
		}
		return true;
	}
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
