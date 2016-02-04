#ifndef INSTRO_PASS_SELECTOR_IDENTIFIERMATCHERSELECTOR_H
#define INSTRO_PASS_SELECTOR_IDENTIFIERMATCHERSELECTOR_H

#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Selector {

class IdentifierMatcherSelector : public InstRO::Core::PassImplementation {
 protected:
	std::vector<std::string> rules;
	IdentifierMatcherSelector() = delete;

 public:
	IdentifierMatcherSelector(std::vector<std::string> matchRules) : PassImplementation(), rules(matchRules){};
	virtual void execute() override;

 protected:
};
}
}

#endif	// INSTRO_CORE_IDENTIFYERSELECTOR_H
