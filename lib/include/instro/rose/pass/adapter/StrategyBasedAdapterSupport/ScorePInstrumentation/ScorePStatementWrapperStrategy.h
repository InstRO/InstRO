#ifndef SCORE_P_STATEMENT_WRAPPER_STRATEGY_H__
#define SCORE_P_STATEMENT_WRAPPER_STRATEGY_H__


#include "instro/adapterbuilder/GenericInstrumentationStrategy.h"

#include <string>
#include <map>

#include "ScorePInitializer.h"

namespace InstRO {

/**
 * \brief Class for ScoreP based instrumentation strategy
 *
 *\author Matthias Zoellner
 */
class ScorePStatementWrapperStrategy : public GenericInstrumentationStrategy {
public:
	ScorePStatementWrapperStrategy(ScorePInitializer* toolInitializerStrategy);

	virtual bool isImplementedFor(SgNode* node);

	virtual ~ScorePStatementWrapperStrategy();

protected:
	// ScoreP needs the following actions to instrument statements:
	// - include scorep/SCOREP_User.h (ToolInitializer)
	// - define handle in global scope of main function
	// - initialize handle in main function
	// - declare handle in global scope of current file
	// - (surround statement with try/catch)
	// - insert exit statements
	// - insert enter statement

	virtual std::vector<SgDeclarationStatement*> constructDeclarationInMainFileFor(SgNode* node);

	virtual std::vector<SgDeclarationStatement*> constructDeclarationInLocalFileFor(SgNode* node);

	virtual std::vector<SgStatement*> constructInitializationInMainScopeFor(SgNode* node);

	virtual std::vector<SgStatement*> constructEntryInstrumentationBeforeNodeFor(SgNode* node);

	virtual std::vector<SgStatement*> constructExitInstrumentationAfterNodeFor(SgNode* node);

	bool isLoopStatement(SgNode* node);

private:
	// create a unique regionname for this node - always returns the same output for same input
	std::string regionName(SgNode* node);

	// create a unique handlename for this node - always returns the same output for same input
	// in contrast to regionname, the handlename is guaranteed to be a valid C variable name
	std::string regionHandleName(SgNode* node);

	// create a unique countername for this node - always returns the same output for same input
	// in contrast to regionname, the countername is guaranteed to be a valid C variable name
	std::string regionCounterName(SgNode* node);

	// get the specific loop name (for, while, do-while, ...) as string for a specified loop node
	std::string loopName(SgNode* node);

	// get the index of this loop node when counting all nodes of same type inside same function
	// in a top-down traversal
	int loopNodeIndex(SgNode* node);

	// generated name storage is needed to use the same variable names for region start and region end
	// instrumentation calls
	std::map<SgNode*, std::string> nameHistory;
};

}

#endif // SCORE_P_STATEMENT_WRAPPER_STRATEGY_H__

