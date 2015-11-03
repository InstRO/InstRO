#include "rose.h"
#include "instro/core/Instrumentor.h"
#include "instro/tooling/NamedConstructAccess.h"
#include "instro/rose/pass/selector/NameBasedSelector.h"
#include "instro/tooling/AnalysisInterface.h"
#include "instro/core/Singleton.h"

namespace InstRO {
namespace Rose {
namespace Selector {

void NameBasedSelector::init() {
	// CI: Nothing to do here.
}
void NameBasedSelector::execute() {
	Tooling::NamedConstructAccess::NamedConstructAccess *nca =
			getInstrumentorInstance()->getAnalysisManager()->getNamedConstructAccessFacility();
	std::list<std::string> matchingRules(rules.begin(), rules.end());
	Tooling::NamedConstructAccess::WildcardedStringMatcher *stringMatcher =
			new ::InstRO::Tooling::NamedConstructAccess::WildcardedStringMatcher(matchingRules);
	outputSet = nca->getConstructsByIdentifyerName(*stringMatcher);
}

void NameBasedSelector::finalize() {}

}
}
}
