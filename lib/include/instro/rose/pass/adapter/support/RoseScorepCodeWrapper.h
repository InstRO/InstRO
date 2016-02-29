#ifndef INSTRO_ROSE_PASS_ADAPTER_SUPPORT_ROSESCOREPCODEWRAPPER_H
#define INSTRO_ROSE_PASS_ADAPTER_SUPPORT_ROSESCOREPCODEWRAPPER_H

#include <string>
#include <map>

#include "rose.h"


namespace InstRO {
namespace Rose {
namespace Adapter {
namespace Support {

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
	std::string generateRegionBeginString(std::string handleName, std::string identifier);
	std::string generateRegionEndString(std::string handleName);
	void handleReturnStatements(SgScopeStatement *scope, std::string identifier);
	void handleRelevantExits(SgScopeStatement *scope, std::string identifier);

	// helper
	std::string generateRegionHandle(SgLocatedNode *node);
	std::string generateDeclareHandleString(std::string handleName);
};

} // Support
} // Adapter
} // Rose
} // InstRO
#endif
