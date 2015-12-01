#ifndef INSTRO_ROSE_PASS_ADAPTER_STRATEGYBASEDADAPTER_GENERIC_INSTRUMENTATION_STRATEGY_H__
#define INSTRO_ROSE_PASS_ADAPTER_STRATEGYBASEDADAPTER_GENERIC_INSTRUMENTATION_STRATEGY_H__

#include <set>
#include <stack>

#include "rose.h"

#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ToolInitializer.h"

namespace InstRO {
namespace Rose {
namespace Adapter {
namespace StrategyBasedAdapterSupport{
/**
	* \brief Base class for generic node instrumentation strategy
	*
	*\author Matthias Zoellner
	*/
class GenericInstrumentationStrategy {
public:
	GenericInstrumentationStrategy(std::shared_ptr<ToolInitializer> toolInitializerStrategy);

	/**
		* override this function to specify, which node types are supported by the strategy
		* returns true, if an instrumentation is available for the specified node
		*/
	virtual bool isImplementedFor(SgNode* node);

	/**
		* Inserts all instrumentor calls according to this strategy
		*/
	virtual void applyInstrumentationFor(SgNode* node);

	virtual ~GenericInstrumentationStrategy();

protected:
	// instrumentation markers
	// variants:

	// Tool setup is delegated to the ToolInitializer
	// - Setup tool specific declarations in main global scope
	std::vector<SgDeclarationStatement*> constructToolDeclarationInMainFile();
	void includeToolHeadersToMainFile();
	// - Setup tool specific declarations in file global scope
	std::vector<SgDeclarationStatement*> constructToolDeclarationInLocalFile(SgGlobal* fileScope);
	void includeToolHeadersToLocalFile(SgGlobal* fileScope);
	// - Initialize tool in main local scope
	std::vector<SgStatement*> constructToolInitializationInMainFunction();
	// - Spin down tool at exit points of main function
	std::vector<SgStatement*> constructToolShutdownInMainFunction();

	// Node specific operations are delegated to subclass
	// - Declare variable for node in main global scope
	virtual std::vector<SgDeclarationStatement*> constructDeclarationInMainFileFor(SgNode* n);
	// - Declare variable for node in file global scope
	virtual std::vector<SgDeclarationStatement*> constructDeclarationInLocalFileFor(SgNode* n);
	// - Declare variable for node in function local scope
	virtual std::vector<SgStatement*> constructDeclarationInFunctionScopeFor(SgNode* n);
	// - Initialize variable for node in main local scope
	virtual std::vector<SgStatement*> constructInitializationInMainScopeFor(SgNode* n);
	// - Initialize variable for node in function local scope
	virtual std::vector<SgStatement*> constructInitializationInFunctionScopeFor(SgNode* n);
	// - Initialize variable for node right before node
	virtual std::vector<SgStatement*> constructInitializationLocallyFor(SgNode* n);
	// - Entry point for node right before node
	virtual std::vector<SgStatement*> constructEntryInstrumentationBeforeNodeFor(SgNode* n);
	// - Entry point for node at start of inner scope
	virtual std::vector<SgStatement*> constructEntryInstrumentationInsideNthScopeFor(SgScopeStatement* node, size_t scopeIndex = 0);
	// - Exit point for node after node and at inner return points
	virtual std::vector<SgStatement*> constructExitInstrumentationAfterNodeFor(SgNode* n);
	// - Exit point for node at end of inner scope and at inner return points
	virtual std::vector<SgStatement*> constructExitInstrumentationInsideNthScopeFor(SgScopeStatement* node, size_t scopeIndex = 0);
	// - Spin down for node at exit points of main function
	virtual std::vector<SgStatement*> constructShutdownInMainFunctionFor(SgNode* n);

private:

	// Tool specific setup may be a once or per file action, spanning mulitple
	// instrumentation strategies.
	// To avoid redundant setup, a separate setup handler is injected into the
	// instrumentation strategy object.
	std::shared_ptr<ToolInitializer> toolInitializer;
};

}
}
}
}
#endif // GENERIC_INSTRUMENTATION_STRATEGY_H__

