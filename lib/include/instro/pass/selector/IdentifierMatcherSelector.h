#ifndef INSTRO_PASS_SELECTOR_IDENTIFIERMATCHERSELECTOR_H
#define INSTRO_PASS_SELECTOR_IDENTIFIERMATCHERSELECTOR_H

#include "instro/core/PassImplementation.h"

#include <string>
#include <vector>

namespace InstRO {
namespace Selector {

/**
 * Matches any of the following against the matchRules list:
 * - variable references
 * - function names.
 * As such, the selector can match appearances of variable names within expressions, such as res = a + b;
 * Note: The matching does not consider declarations of variable names. Consequently int a = 42 would not be matched.
 */
class IdentifierMatcherSelector : public InstRO::Core::PassImplementation {
 protected:
	std::vector<std::string> rules;
	IdentifierMatcherSelector() = delete;

 public:
	IdentifierMatcherSelector(std::vector<std::string> matchRules) : PassImplementation(), rules(matchRules){};
	virtual void execute() override;

 protected:
};
}	// namespace Selector
}	// namespace InstRO

#endif	// INSTRO_CORE_IDENTIFYERSELECTOR_H
