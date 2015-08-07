#include "instro/core/Instrumentor.h"
#include "instro/tooling/NamedConstructAccess.h"
#include "instro/example/NameBasedSelector.h"
#include "instro/tooling/AnalysisInterface.h"
#include "instro/core/Singleton.h"

namespace InstRO {
namespace Example {
namespace Selectors {

void NameBasedSelector::init() {
	// CI: Nothing to do here.
}
void NameBasedSelector::execute() {
	Tooling::NamedConstructAccess::NamedConstructAccess *nca =
			getInstrumentorInstance()->getAnalysisManager()->getNamedConstructAccessFacility();
	std::list<std::string> matchingRules(rules.begin(), rules.end());
	Tooling::NamedConstructAccess::WildcardedStringMatcher *stringMatcher =
			new ::InstRO::Tooling::NamedConstructAccess::WildcardedStringMatcher(matchingRules);
	std::unique_ptr<InstRO::Core::ConstructSet> cs = nca->getConstructsByIdentifyerName(*stringMatcher);
	output = *cs;
}

void NameBasedSelector::finalize() {}

void NameBasedSelector::releaseOutput() { output.clear(); }

Core::ConstructSet *NameBasedSelector::getOutput() { return &output; }
}
}
}
