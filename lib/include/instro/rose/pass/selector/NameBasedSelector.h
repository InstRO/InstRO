#include "instro/rose/core/RosePassImplementation.h"

namespace InstRO {
namespace Rose {
namespace Selector {

class NameBasedSelector : public InstRO::Rose::RosePassImplementation {
 protected:
	Pass *filterChannel;
	std::vector<std::string> rules;
	NameBasedSelector() = delete;
	std::unique_ptr<Core::ConstructSet> output;

 public:
	NameBasedSelector(std::vector<std::string> matchRules, Pass *filter)
			: RosePassImplementation(Core::ChannelConfiguration(filter)), filterChannel(filter), rules(matchRules){};
	NameBasedSelector(std::vector<std::string> matchRules)
			: RosePassImplementation(Core::ChannelConfiguration()), filterChannel(NULL), rules(matchRules){};
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