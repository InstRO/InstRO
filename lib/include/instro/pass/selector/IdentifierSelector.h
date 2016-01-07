#ifndef INSTRO_PASS_SELECTOR_IDENTIFIERSELECTOR_H
#define INSTRO_PASS_SELECTOR_IDENTIFIERSELECTOR_H

#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Selector {

class IdentifyerSelector : public InstRO::Core::PassImplementation {
 protected:
	std::vector<std::string> rules;
	IdentifyerSelector() = delete;

 public:
	IdentifyerSelector(std::vector<std::string> matchRules, Pass *filter)
			: PassImplementation(Core::ChannelConfiguration(filter)), rules(matchRules){};
	IdentifyerSelector(std::vector<std::string> matchRules)
			: PassImplementation(Core::ChannelConfiguration()), rules(matchRules){};
	virtual void execute() override;

 protected:
};
}
}

#endif	// INSTRO_CORE_IDENTIFYERSELECTOR_H
