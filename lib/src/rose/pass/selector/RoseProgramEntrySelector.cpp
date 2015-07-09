#include "instro/core/Singleton.h"
#include "instro/tooling/GrammarInterface.h"
#include "instro/tooling/NamedConstructAccess.h"
#include "instro/rose/pass/selector/RoseProgramEntrySelector.h"

#include <string>
#include <list>

namespace InstRO {
namespace Rose {
namespace Selector {
void ProgramEntrySelector::init() {}
void ProgramEntrySelector::finalize() {}
void ProgramEntrySelector::execute() {
	auto namedConstructAccessFacility =
			InstRO::getInstrumentorInstance()->getAnalysisManager()->getNamedConstructAccessFacility();
	auto grammarInterface = InstRO::getInstrumentorInstance()->getAnalysisManager()->getGrammarInterface();

	std::list<std::string> mainRegEx;
	mainRegEx.push_back(std::string("main"));
	InstRO::Tooling::NamedConstructAccess::WildcardedStringMatcher mainMatcher(mainRegEx);
	std::unique_ptr<InstRO::Core::ConstructSet> csByNameMain =
			namedConstructAccessFacility->getConstructsByIdentifyerName(mainMatcher);
	// we now have anything that is called main. intersect it with the set of function statements
	std::unique_ptr<InstRO::Core::ConstructSet> csFunctions =
			grammarInterface->getConstructsByType(InstRO::Tooling::GrammarInterface::GTFunction);
	output = std::make_unique<InstRO::Core::ConstructSet>(csFunctions->intersect(*(csByNameMain.get())));
}
Core::ConstructSet* ProgramEntrySelector::getOutput() { return output.get(); }
void ProgramEntrySelector::releaseOutput() { output->clear(); }

}	// Selector
}	// Rose
}	// InstRO