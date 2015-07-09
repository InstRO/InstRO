#include "instro/tooling/NamedConstructAccess.h"
#include "instro/pass/selector/TextStringSelector.h"
#include "instro/tooling/AnalysisInterface.h"
#include "instro/core/Singleton.h"

namespace InstRO {
namespace Selector {

void TextStringSelector::init() {
	// CI: Nothing to do here.
}
void TextStringSelector::execute() {
	Tooling::NamedConstructAccess::NamedConstructAccess *nca =
			getInstrumentorInstance()->getAnalysisManager()->getNamedConstructAccessFacility();
	std::list<std::string> matchingRules(rules.begin(), rules.end());
	Tooling::NamedConstructAccess::WildcardedStringMatcher *stringMatcher =
			new ::InstRO::Tooling::NamedConstructAccess::WildcardedStringMatcher(matchingRules);
	output = nca->getConstructsByUserTextStringMatch(*stringMatcher);
}

void TextStringSelector::finalize() {}

void TextStringSelector::releaseOutput() { output->clear(); }

Core::ConstructSet *TextStringSelector::getOutput() { return output.get(); }
}
}
