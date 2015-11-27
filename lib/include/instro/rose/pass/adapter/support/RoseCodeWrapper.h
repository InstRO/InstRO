#ifndef INSTRO_ROSE_PASS_ADAPTER_SUPPORT_ROSECODEWRAPPER_H_
#define INSTRO_ROSE_PASS_ADAPTER_SUPPORT_ROSECODEWRAPPER_H_

#include <rose.h>

namespace InstRO {
namespace Rose {
namespace Adapter {
namespace Support {

/**
 * \brief The entity that does the actual code instrumentation in Rose.
 * For now only the InstRO measurement interface is supported.
 * XXX RN 2015-11: currently rose is not able to unparse headers. Therefore instrumentation in headers is not supported
 * TODO RN: a bad order of instrumentations may lead to a corrupt nesting of instrumentation hooks
 *  * \author Roman Ness
 */
class RoseCodeWrapper {
public:
	RoseCodeWrapper(SgProject* project) : project(project) {}

	void wrapStatement(SgStatement* stmt, std::string postfix, size_t id);
	void wrapExpression(SgExpression* expr, size_t id);
	void instrumentFunction(SgFunctionDefinition* function, size_t id);
	void instrumentScope(SgScopeStatement* scope, size_t id);

private:
	SgProject* project;

	std::set<SgSourceFile*> filesWithInclude;

	void instrumentPossibleExit(SgScopeStatement* scope, SgStatement* exit, SgStatement* instrumentStmt);
	bool insertHeaderIfSource(SgLocatedNode* node);
	SgStatement* buildCallExpressionStatement(SgScopeStatement* context, std::string functionName, SgExprListExp* parameters);
	SgStatement* buildCallExpressionStatement(SgScopeStatement* context, std::string functionName, size_t id);

};

}	// namespace Support
}	// namespace Adapter
}	// namespace Rose
}	// namespace InstRO


#endif /* INSTRO_ROSE_PASS_ADAPTER_SUPPORT_ROSECODEWRAPPER_H_ */
