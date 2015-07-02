#include "instro/rose/RosePass.h"

namespace InstRO {
	namespace Rose{
		namespace Selectors
		{

			class NameBasedSelector:public InstRO::Rose::RosePass {
			protected:
				Pass * filterChannel;
				std::vector<std::string> rules;
				NameBasedSelector() = delete;
				Core::ConstructSet output;
			public:
				NameBasedSelector(std::vector<std::string> matchRules, Pass * filter) :RosePass(Core::ChannelConfiguration(filter)), filterChannel(filter), rules(matchRules){
				};
				NameBasedSelector(std::vector<std::string> matchRules) :RosePass(Core::ChannelConfiguration()), filterChannel(NULL), rules(matchRules){
				};
				virtual void init() override;
				virtual void execute() override;
				virtual void finalize() override;
				virtual void releaseOutput() override;
				virtual Core::ConstructSet *getOutput() override;
			protected:
			};
		}
	}
}