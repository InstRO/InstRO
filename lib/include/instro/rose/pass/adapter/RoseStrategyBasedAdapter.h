#ifndef INSTRO_ROSE_STRATEGYBASEDADAPTER
#define INSTRO_ROSE_STRATEGYBASEDADAPTER
#include "instro/core/Pass.h"
#include "instro/rose/core/RosePassImplementation.h"

#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/GenericInstrumentationStrategy.h"
#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ToolInitializer.h"


// InstRO::Rose::Adapter::StrategyBasedAdapterSupport

namespace InstRO {
	namespace Rose {
		namespace Adapter {

			class RoseStrategyBasedAdapter : public RosePassImplementation {
			private:
				// priority-list
				std::multimap<int, InstRO::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy*, std::greater<int> > strategies;
				typedef std::multimap<int, InstRO::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy*, std::greater<int> >::iterator strat_iterator;

			protected:
				InstRO::Pass *input;
				InstRO::Core::ConstructSet outputCS;

			public:
				template<class... stratList> RoseStrategyBasedAdapter(InstRO::Pass *inputFunctions, InstRO::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy* strat1, stratList... strategyArgs)
					: RosePassImplementation(InstRO::Core::ChannelConfiguration(inputFunctions)), input(inputFunctions)
				{
					strategies.insert(std::make_pair<1, strat1>);
					int prio = 2;
					std::vector<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy*> strategyVec = { strategyArgs... };
					for (auto strat : strategyVec)
					{
						strategies.insert(std::make_pair<prio, strat>);
						prio++;
						//.insert(inputChannelPasses.begin(), { p1, passes... });
					}
				}
				void init(){};
				void execute() override;
				void finalize(){};
				void releaseOutput() { outputCS.clear(); };
				InstRO::Core::ConstructSet *getOutput() { return &outputCS; }
			};
		}
	}
}
#endif
