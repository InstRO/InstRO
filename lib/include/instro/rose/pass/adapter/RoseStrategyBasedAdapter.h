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
				RoseStrategyBasedAdapter(InstRO::Pass *inputFunctions)
					: RosePassImplementation(InstRO::Core::ChannelConfiguration(inputFunctions)), input(inputFunctions) {}
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
