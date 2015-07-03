#include "instro/core/ConstructSet.h"
#include "instro/example/ExamplePass.h"

namespace InstRO {
	namespace Example{
		namespace Selectors
		{

			class NameBasedSelector:public InstRO::Example::ExamplePass {
			protected:
				Pass * filterChannel;
				std::vector<std::string> rules;
				NameBasedSelector() = delete;
				InstRO::Core::ConstructSet output;
			public:
				NameBasedSelector(std::vector<std::string> matchRules, Pass * filter) :ExamplePass(InstRO::Core::ChannelConfiguration(filter)), filterChannel(filter), rules(matchRules){
				};
				NameBasedSelector(std::vector<std::string> matchRules) :ExamplePass(InstRO::Core::ChannelConfiguration()), filterChannel(NULL), rules(matchRules){
				};
				virtual void init() override;
				virtual void execute() override;
				virtual void finalize() override;
				virtual void releaseOutput() override;
				virtual InstRO::Core::ConstructSet *getOutput() override;
			protected:
			};
		}
	}
}