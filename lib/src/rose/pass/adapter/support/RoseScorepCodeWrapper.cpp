#include "instro/rose/pass/adapter/support/RoseScorepCodeWrapper.h"

#include "instro/rose/utility/ASTTransformer.h"
#include "instro/utility/Logger.h"

namespace InstRO {
namespace Rose {
namespace Adapter {
namespace Support {

void RoseScorepCodeWrapper::wrapStatement(SgLocatedNode* node, std::string identifier) {
	auto& helper = Utility::ASTTransformation::HeaderIncludeHelper::getInstance();
	if (helper.insertHeaderIfSource("scorep/SCOREP_User.h", node)) {
		std::string handleName = declareHandleIfNecessary(node);

		SageInterface::attachArbitraryText(node, generateRegionBeginString(handleName, identifier),
																			 PreprocessingInfo::before);
		if(isSgScopeStatement(node)){
			handleRelevantExits(isSgScopeStatement(node), identifier);
		}
		
		SageInterface::attachArbitraryText(node, generateRegionEndString(handleName), PreprocessingInfo::after);
	}
}

void RoseScorepCodeWrapper::wrapExpression(SgLocatedNode* node, std::string identifier) {
	// TODO Implement me
	logIt(ERROR) << "RoseArbitraryTextCodeWrapper::wrapExpression not implemented" << std::endl;
}

/**
 * FIXME Handle exits of scopes (break, continue, return)
 */
void RoseScorepCodeWrapper::instrumentScope(SgScopeStatement* scope, std::string identifier) {
	auto& helper = Utility::ASTTransformation::HeaderIncludeHelper::getInstance();
	if (helper.insertHeaderIfSource("scorep/SCOREP_User.h", scope)) {
		std::string handleName = declareHandleIfNecessary(scope);

		auto firstStmt = SageInterface::getFirstStatement(scope);
		if (firstStmt != nullptr) {
			auto anchor = SageBuilder::buildNullStatement();
			SageInterface::prependStatement(anchor, scope);
			SageInterface::attachArbitraryText(anchor, generateRegionBeginString(handleName, identifier),
																				 PreprocessingInfo::before);
		}

		handleRelevantExits(scope, identifier);

		auto anchor = SageBuilder::buildNullStatement();
		SageInterface::appendStatement(anchor, scope);
		SageInterface::attachArbitraryText(anchor, generateRegionEndString(handleName), PreprocessingInfo::before);
	}
}

void RoseScorepCodeWrapper::instrumentFunction(SgFunctionDefinition* node, std::string identifier) {
	auto& helper = Utility::ASTTransformation::HeaderIncludeHelper::getInstance();
	if (helper.insertHeaderIfSource("scorep/SCOREP_User.h", node)) {
		auto anchor = SageBuilder::buildNullStatement();
		SgScopeStatement *fBody = node->get_body();
		SageInterface::prependStatement(anchor, fBody);

		auto firstStmt = SageInterface::getFirstStatement(fBody);
		
		if (firstStmt != nullptr) {
			std::string handleName = declareHandleIfNecessary(fBody);
			SageInterface::attachArbitraryText(anchor, generateRegionBeginString(handleName, identifier),
																				 PreprocessingInfo::before);
		}

		handleReturnStatements(fBody, identifier);
	}
}

void RoseScorepCodeWrapper::handleRelevantExits(SgScopeStatement *scope, std::string identifier){
	auto handleName = declareHandleIfNecessary(scope);

	for(auto n : SageInterface::findBreakStmts(scope)){
		SageInterface::attachArbitraryText(n, generateRegionEndString(handleName), PreprocessingInfo::before);
	}

	for(auto n : SageInterface::findContinueStmts(scope)){
		SageInterface::attachArbitraryText(n, generateRegionEndString(handleName), PreprocessingInfo::before);
	}

	handleReturnStatements(scope, identifier);
}

void RoseScorepCodeWrapper::handleReturnStatements(SgScopeStatement* scope, std::string identifier) {
	auto returns = SageInterface::querySubTree<SgReturnStmt>(scope);
	auto handleName = declareHandleIfNecessary(scope);
	for (auto n : returns) {
		auto returnExpr = n->get_expression();
		if (returnExpr != nullptr && !(isSgValueExp(returnExpr)) && !(isSgVarRefExp(returnExpr)) &&
				!(isSgNullExpression(returnExpr))) {
			SageInterface::splitExpression(returnExpr);	// return statement transformation
		}
	
		SageInterface::attachArbitraryText(n, generateRegionEndString(handleName), PreprocessingInfo::before);
	}
}

std::string RoseScorepCodeWrapper::declareHandleIfNecessary(SgLocatedNode* node) {
	if (handlesGenerated.find(node) == handlesGenerated.end()) {
		std::string handle = generateRegionHandle(node);

		auto encFunction = SageInterface::getEnclosingFunctionDefinition(node, true);	// include "me"
		auto firstStmt = SageInterface::getFirstStatement(encFunction);
		SageInterface::attachArbitraryText(firstStmt, generateDeclareHandleString(handle), PreprocessingInfo::before);

		handlesGenerated.insert({node, handle});
		return handle;
	}

	return handlesGenerated.at(node);
}

std::string RoseScorepCodeWrapper::generateRegionBeginString(std::string handleName, std::string identifier) {
	return std::string("SCOREP_USER_REGION_BEGIN( ") + handleName + std::string(", \"") + identifier +
				 std::string("\", SCOREP_USER_REGION_TYPE_COMMON)");
}

std::string RoseScorepCodeWrapper::generateRegionEndString(std::string handleName) {
	return std::string("SCOREP_USER_REGION_END( ") + handleName + std::string(")");
}

std::string RoseScorepCodeWrapper::generateRegionHandle(SgLocatedNode* node) {
	return "instro_reg_handle_" + std::to_string((size_t)node);
}

std::string RoseScorepCodeWrapper::generateDeclareHandleString(std::string handleName) {
	return std::string("SCOREP_USER_REGION_DEFINE( ") + handleName + std::string(")");
}
}
}
}
}
