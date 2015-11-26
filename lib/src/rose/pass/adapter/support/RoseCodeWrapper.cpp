#include "instro/rose/pass/adapter/support/RoseCodeWrapper.h"

#include "instro/utility/Logger.h"

///XXX
#include <cassert>

namespace InstRO {
namespace Rose {
namespace Adapter {
namespace Support {

void RoseCodeWrapper::wrapStatement(SgStatement* stmt, size_t id) {
	if (insertHeaderIfSource(stmt)) {
		auto functionScope = SageInterface::getScope(stmt);
		SageInterface::insertStatementBefore(stmt, buildCallExpressionStatement(functionScope,"enter",id));
		SageInterface::insertStatementAfter(stmt, buildCallExpressionStatement(functionScope,"exit",id));
	}
}
void RoseCodeWrapper::wrapExpression(SgExpression* expr, size_t id) {
	if (insertHeaderIfSource(expr)) {

	}
	// TODO implement me
	logIt(ERROR) << "RoseCodeWrapper: wrapExpression() not implemented." << std::endl;
}
void RoseCodeWrapper::instrumentFunction(SgFunctionDefinition* function, size_t id) {
	if (insertHeaderIfSource(function)) {

		SageInterface::prependStatement(buildCallExpressionStatement(function->get_body(), "enter", id), function->get_body());
		SageInterface::instrumentEndOfFunction(function->get_declaration(), buildCallExpressionStatement(function, "exit", id));
	}
}

bool RoseCodeWrapper::insertHeaderIfSource(SgLocatedNode* node) {
	SgFile* file = SageInterface::getEnclosingFileNode(node);
	SgSourceFile* sourceFile = isSgSourceFile(file);
	if (sourceFile != nullptr) {

		if (filesWithInclude.find(sourceFile) == filesWithInclude.end()) {
			SageInterface::insertHeader("InstROMeasurementInterface.h", PreprocessingInfo::before, true, sourceFile->get_globalScope());
			filesWithInclude.insert(sourceFile);
		}
		return true;
	}
	return false;
}

SgStatement* RoseCodeWrapper::buildCallExpressionStatement(SgScopeStatement* context, std::string functionName, size_t id) {
	SgExpression* call = SageBuilder::buildFunctionCallExp(
			functionName, SageBuilder::buildVoidType(),
			SageBuilder::buildExprListExp(SageBuilder::buildUnsignedLongLongIntVal(id)), context);

	return SageBuilder::buildExprStatement(call);
}

}	// namespace Support
}	// namespace Adapter
}	// namespace Rose
}	// namespace InstRO

