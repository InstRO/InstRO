#ifndef INSTRO_ROSE_PASS_ADAPTER_SUPPORT_ROSESCOREPCODEWRAPPER_H
#define INSTRO_ROSE_PASS_ADAPTER_SUPPORT_ROSESCOREPCODEWRAPPER_H

#include <string>
#include <map>

#include "rose.h"

namespace InstRO {
namespace Rose {
namespace Adapter {
namespace Support {

/**
 * Wraps constructs within Scorep user region Macros. The registration order and placement of the individual identifiers
 * is not specified and can be within the scope of another measurement hook.
 *
 * \author JP Lehr
 */
class RoseScorepCodeWrapper {
 public:
	RoseScorepCodeWrapper(SgProject *p) : project(p) {}

	void wrapStatement(SgLocatedNode *node, std::string identifier);
	void wrapExpression(SgLocatedNode *node, std::string identifier);
	void instrumentScope(SgScopeStatement *node, std::string identifier);
	void instrumentFunction(SgFunctionDefinition *node, std::string identifier);

 private:
	SgProject *project;

	std::map<SgLocatedNode *, std::string> handlesGenerated;

	// generates and declares if necessary
	std::string declareHandleIfNecessary(SgLocatedNode *node);
	void handleReturnStatements(SgScopeStatement *scope, std::string identifier);
	void handleRelevantExits(SgScopeStatement *scope, std::string identifier);

	// helper
	std::string generateRegionHandle(SgLocatedNode *node);
	std::string generateDeclareHandleMacro(std::string handleName);
	std::string generateRegionBeginMacro(std::string handleName, std::string identifier);
	std::string generateRegionEndMacro(std::string handleName);
	SgStatement* prependAnchor(SgScopeStatement *scope);	
	SgStatement* appendAnchor(SgScopeStatement *scope);
};

}	// Support
}	// Adapter
}	// Rose
}	// InstRO
#endif
