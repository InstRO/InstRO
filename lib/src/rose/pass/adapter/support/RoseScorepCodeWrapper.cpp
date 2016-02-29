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

		SageInterface::attachArbitraryText(node, generateRegionBeginString(handleName, identifier), PreprocessingInfo::before);
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
void RoseScorepCodeWrapper::instrumentScope(SgScopeStatement* node, std::string identifier) {
	auto& helper = Utility::ASTTransformation::HeaderIncludeHelper::getInstance();
	if (helper.insertHeaderIfSource("scorep/SCOREP_User.h", node)) {
		std::string handleName = declareHandleIfNecessary(node);

		auto firstStmt = SageInterface::getFirstStatement(node);
		auto lastStmt = SageInterface::getLastStatement(node);
		if (firstStmt != nullptr) {
			SageInterface::attachArbitraryText(firstStmt, generateRegionBeginString(handleName, identifier), PreprocessingInfo::before);
		}
		if (lastStmt != nullptr) {
			SageInterface::attachArbitraryText(lastStmt, generateRegionEndString(handleName), PreprocessingInfo::after);
		}
	}
}

/**
 * TODO A function isn't necessarily a scope in the sense of instrumentation
 */
void RoseScorepCodeWrapper::instrumentFunction(SgFunctionDefinition* node, std::string identifier) {
	auto& helper = Utility::ASTTransformation::HeaderIncludeHelper::getInstance();
	if (helper.insertHeaderIfSource("scorep/SCOREP_User.h", node)) {
		auto fBody = node->get_body();
		instrumentScope(fBody, identifier);
	}
}


std::string RoseScorepCodeWrapper::declareHandleIfNecessary(SgLocatedNode *node){
	if(handlesGenerated.find(node) == handlesGenerated.end()){
		std::string handle = generateRegionHandle(node);
		
		auto encFunction = SageInterface::getEnclosingFunctionDefinition(node, true); // include "me"
		auto firstStmt = SageInterface::getFirstStatement(encFunction);
		SageInterface::attachArbitraryText(firstStmt, generateDeclareHandleString(handle), PreprocessingInfo::before);

		handlesGenerated.insert({node, handle});
		return handle;
	}
	return handlesGenerated.at(node);
}

std::string RoseScorepCodeWrapper::generateRegionBeginString(std::string handleName, std::string identifier){
	return std::string("SCOREP_USER_REGION_BEGIN( ") + handleName + std::string(", \"") + identifier +
				 std::string("\", SCOREP_USER_REGION_TYPE_COMMON)");
}

std::string RoseScorepCodeWrapper::generateRegionEndString(std::string handleName){
	return std::string("SCOREP_USER_REGION_END( ") + handleName + std::string(")");
}

std::string RoseScorepCodeWrapper::generateRegionHandle(SgLocatedNode *node){
	return "instro_reg_handle_" + std::to_string((size_t)node);
}

std::string RoseScorepCodeWrapper::generateDeclareHandleString(std::string handleName){
	return std::string("SCOREP_USER_REGION_DEFINE( ") + handleName + std::string(")");
}

}
}
}
}
