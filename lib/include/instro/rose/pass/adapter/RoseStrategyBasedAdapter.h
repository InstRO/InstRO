#ifndef INSTRO_ROSE_STRATEGYBASEDADAPTER
#define INSTRO_ROSE_STRATEGYBASEDADAPTER
#include <memory>
#include <utility>
#include "include/instro/utility/MemoryManagement.h"
#include "instro/core/Pass.h"
#include "instro/rose/core/RosePassImplementation.h"

#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePLoopIterationStrategy.h"
#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePStatementWrapperStrategy.h"
#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePFunctionScopeStrategy.h"
#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/GenericInstrumentationStrategy.h"
#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ToolInitializer.h"

namespace InstRO {
	namespace Rose {
		namespace Adapter {

			class RoseStrategyBasedAdapter : public RosePassImplementation {
			private:
				// priority-list
				std::vector< std::unique_ptr<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy> > strategyUniqPtrs;
				std::multimap<int, InstRO::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy*, std::greater<int> > strategies;
				typedef std::multimap<int, InstRO::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy*, std::greater<int> >::iterator strat_iterator;

			protected:
//				InstRO::Pass *input;
				template<class... stratList> void ctorStrategyInserter(std::unique_ptr<InstRO
						::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy> && strat1, stratList&&... strategyArgs){
					strategyUniqPtrs.push_back(std::move(strat1));
					ctorStrategyInserter(std::forward<stratList>(strategyArgs)...);
					return ;
				}
				void ctorStrategyInserter()
{
}
			public:
				RoseStrategyBasedAdapter() = delete;
				template<class... stratList> RoseStrategyBasedAdapter(std::unique_ptr<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy> && strat1, stratList&&... strategyArgs)
					: RosePassImplementation()
				{
					ctorStrategyInserter(std::move(strat1),std::forward<stratList>(strategyArgs)...);
//					strategyUniqPtrs.push_back(std::move(strat1));
//					strategyUniqPtrs.insert(strategyUniqPtrs.end(),std::initializer_list<(std::forward<stratList>(strategyArgs)...));

//					strategyUniqPtrs.insert(strategyUniqPtrs.end(),{std::forward<stratList>(strategyArgs)...});
//					strategies.insert(std::make_pair(1, strat1.get()));
					int prio = 1;
//					std::vector<std::uniq_ptr<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy>> strategyVec = { strategyArgs... };
					for (auto & strat : strategyUniqPtrs)
					{
						strategies.insert(std::make_pair(prio, strat.get()));
//						strategyUniqPtrs.push_back(std::move(strat));
						prio++;
						//.insert(inputChannelPasses.begin(), { p1, passes... });
					}
				}
				void execute() override;
			};
		}
	}
}
#endif
