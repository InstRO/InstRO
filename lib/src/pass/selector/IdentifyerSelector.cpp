#include "instro/core/Instrumentor.h"
#include "instro/tooling/NamedConstructAccess.h"
#include "instro/pass/selector/IdentifyerSelector.h"
#include "instro/tooling/AnalysisInterface.h"
#include "instro/core/Singleton.h"

namespace InstRO {
namespace Selector {

void IdentifyerSelector::init() {
	// CI: Nothing to do here.
}
void IdentifyerSelector::execute() {
	Tooling::NamedConstructAccess::NamedConstructAccess *nca =
			getInstrumentorInstance()->getAnalysisManager()->getNamedConstructAccessFacility();
	std::list<std::string> matchingRules(rules.begin(), rules.end());
	Tooling::NamedConstructAccess::WildcardedStringMatcher *stringMatcher =
			new ::InstRO::Tooling::NamedConstructAccess::WildcardedStringMatcher(matchingRules);
	output = nca->getConstructsByIdentifyerName(*stringMatcher);
}

void IdentifyerSelector::finalize() {}

void IdentifyerSelector::releaseOutput() { output->clear(); }

Core::ConstructSet *IdentifyerSelector::getOutput() { return output.get(); }
}
}
