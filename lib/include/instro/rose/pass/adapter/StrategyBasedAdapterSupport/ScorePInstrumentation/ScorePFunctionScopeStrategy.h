#ifndef INSTRO_ROSE_PASS_ADAPTER_STRATEGYBASEDADAPTER_SCOREPINSTRUMENTATION_SCORE_P_FUNCTION_SCOPE_STRATEGY_H__
#define INSTRO_ROSE_PASS_ADAPTER_STRATEGYBASEDADAPTER_SCOREPINSTRUMENTATION_SCORE_P_FUNCTION_SCOPE_STRATEGY_H__


#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/GenericInstrumentationStrategy.h"
#include <memory>
#include <string>
#include <map>

#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePInitializer.h"

namespace InstRO {
	namespace Rose {
		namespace Adapter {
			namespace StrategyBasedAdapterSupport{

				/**
				 * \brief Class for ScoreP based instrumentation strategy
				 *
				 *\author Matthias Zoellner
				 */
				class ScorePFunctionScopeStrategy : public GenericInstrumentationStrategy {
				public:
					ScorePFunctionScopeStrategy(std::shared_ptr<ToolInitializer> initializer);

					virtual bool isImplementedFor(SgNode* node);

					virtual ~ScorePFunctionScopeStrategy();

				protected:
					// ScoreP needs the following actions to instrument statements:
					// - include scorep/SCOREP_User.h
					// - define handle in global scope of main function
					// - initialize handle in main function
					// - declare handle in global scope of current file
					// - (surround statement with try/catch)
					// - insert exit statements
					// - insert enter statement

					// base overrides

					std::vector<SgDeclarationStatement*> constructDeclarationInMainFileFor(SgNode* node);

					std::vector<SgDeclarationStatement*> constructDeclarationInLocalFileFor(SgNode* node);

					std::vector<SgStatement*> constructInitializationInMainScopeFor(SgNode* node);

					std::vector<SgStatement*> constructEntryInstrumentationInsideNthScopeFor(SgScopeStatement* node, size_t index);

					std::vector<SgStatement*> constructExitInstrumentationInsideNthScopeFor(SgScopeStatement* node, size_t index);

					// end base overrides

				private:
					// create a unique regionname for this node - always returns the same output for same input
					std::string regionName(SgNode* node);

					// create a unique handlename for this node - always returns the same output for same input
					// in contrast to regionname, the handlename is guaranteed to be a valid C variable name
					std::string regionHandleName(SgNode* node);

					// generated name storage is needed to use the same variable names for region start and region end
					// instrumentation calls
					std::map<SgNode*, std::string> nameHistory;
				};

			}
		}
	}
}
#endif // SCORE_P_FUNCTION_SCOPE_STRATEGY_H__

