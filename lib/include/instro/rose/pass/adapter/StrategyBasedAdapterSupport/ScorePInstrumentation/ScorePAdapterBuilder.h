#ifndef INSTRO_ROSE_PASS_ADAPTER_STRATEGYBASEDADAPTER_SCOREPINSTRUMENTATION_SCOREP_ADAPTER_BUILDER_H
#define INSTRO_ROSE_PASS_ADAPTER_STRATEGYBASEDADAPTER_SCOREPINSTRUMENTATION_SCOREP_ADAPTER_BUILDER_H


#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/GenericAdapterBuilder.h"

#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePStatementWrapperStrategy.h"
#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePLoopIterationStrategy.h"
#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePFunctionScopeStrategy.h"

#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePInitializer.h"
#if 0
namespace InstRO {
	namespace Rose {
		namespace Adapter {
			namespace StrategyBasedAdapterSupport{

				/**
				 * \brief Composition for ScoreP instrumentation
				 *
				 * \ingroup Adapterbuilder
				 *
				 *\author Matthias Zoellner
				 */
				class ScorePAdapterBuilder : public GenericAdapterBuilder {

				public:
					ScorePAdapterBuilder(Selector* decidingSelector);

					virtual ~ScorePAdapterBuilder();

				private:
					ScorePInitializer initializer;

					ScorePStatementWrapperStrategy my_strategy;
					ScorePLoopIterationStrategy my_strategy2;
					ScorePFunctionScopeStrategy my_strategy3;
				};

			}
		}
	}
}

#endif
#endif

