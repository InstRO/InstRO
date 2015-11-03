#include "instro/rose/core/RosePassImplementation.h"

namespace InstRO {
namespace Rose {
namespace Selector {

class NameBasedSelector : public InstRO::Rose::RosePassImplementation {
 protected:
	Pass *filterChannel;
	std::vector<std::string> rules;
	NameBasedSelector() = delete;

 public:
	NameBasedSelector(std::vector<std::string> matchRules, Pass *filter)
			: RosePassImplementation(InstRO::Core::ChannelConfiguration(filter)), filterChannel(filter), rules(matchRules){};
	NameBasedSelector(std::vector<std::string> matchRules)
			: RosePassImplementation(InstRO::Core::ChannelConfiguration()), filterChannel(NULL), rules(matchRules){};
	virtual void init() override;
	virtual void execute() override;
	virtual void finalize() override;

 protected:
};

}	// Selector
}	// Rose
}	// InstRO
