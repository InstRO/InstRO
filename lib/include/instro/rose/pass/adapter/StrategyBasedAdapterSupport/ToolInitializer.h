#ifndef INSTRO_ROSE_PASS_ADAPTER_STRATEGYBASEDADAPTER_TOOL_INITIALIZER_H__
#define INSTRO_ROSE_PASS_ADAPTER_STRATEGYBASEDADAPTER_TOOL_INITIALIZER_H__

#include <rose.h>

#include <set>
#include <vector>

namespace InstRO {
namespace Rose {
namespace Adapter {
namespace StrategyBasedAdapterSupport{

/**
	* \brief Base class for initialization of a single instrumentation tool (like ScoreP)
	*
	* Intent of this class is to provide an initialization interface for a specific tool.
	* In contrast to instrumentation, the initialization only happens once (for each file)
	* therefore control becomes complicated if the responsibility resides in an
	* instrumentation strategy object.
	*
	* \author Matthias Zoellner
	*/
class ToolInitializer {
public:

	/** Include the tool headers to the provided file */
	virtual void includeToolHeadersToFile(SgFile* file);

	/** Declaration statements to be inserted */
	virtual std::vector<SgDeclarationStatement*> constructToolDeclarationForMainFile(SgFile* main);

	/** Declaration statements to be inserted */
	virtual std::vector<SgDeclarationStatement*> constructToolDeclarationForLocalFile(SgFile* file);

	/** Assumes that the provided scope is executed before any tool usage. For example in main file */
	virtual std::vector<SgStatement*> constructToolInitializationIn(SgScopeStatement* scope);

	/** Assumes that no tool usage appears after the end of this scope */
	virtual std::vector<SgStatement*> constructToolShutdownIn(SgScopeStatement* scope);

	virtual ~ToolInitializer();

protected:
	// Initialization control functions - use them to avoid multiple initializations
	void setIncludedIn(SgFile* file);
	void setDeclaredIn(SgFile* file);
	void setInitializedIn(SgScopeStatement* scope);
	void setShutdownIn(SgScopeStatement* scope);

	bool isIncludedIn(SgFile* file);
	bool isDeclaredIn(SgFile* file);
	bool isInitializedIn(SgScopeStatement* scope);
	bool isShutdownIn(SgScopeStatement* scope);

private:
	std::set<SgFile*> isIncluded;
	std::set<SgFile*> isDeclared;
	std::set<SgScopeStatement*> isInitialized;
	std::set<SgScopeStatement*> isShutdown;

}; // class ToolInitializer

}
}
}
} // namespace InstRO

#endif // TOOL_INITIALIZER_H__

