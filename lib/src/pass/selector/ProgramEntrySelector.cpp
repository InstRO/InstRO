#include "instro/core/Singleton.h"
#include "instro/tooling/GrammarInterface.h"
#include "instro/tooling/NamedConstructAccess.h"
#include "instro/pass/selector/ProgramEntrySelector.h"

#include <string>
#include <list>

namespace InstRO {
namespace Selector {
void ProgramEntrySelector::init() {}
void ProgramEntrySelector::finalize() {}
void ProgramEntrySelector::execute() {
	std::list<std::string> mainRegEx;
	mainRegEx.push_back(std::string("main"));
	auto mainMatcher = InstRO::Tooling::NamedConstructAccess::WildcardedStringMatcher(mainRegEx);

	std::unique_ptr<InstRO::Core::ConstructSet> csByNameMain = InstRO::getInstrumentorInstance()
																																 ->getAnalysisManager()
																																 ->getNamedConstructAccessFacility()
																																 ->getConstructsByIdentifyerName(mainMatcher);
	// we now have anything that is called main. intersect it with the set of function statements
	std::unique_ptr<InstRO::Core::ConstructSet> csFunctions =
			InstRO::getInstrumentorInstance()->getAnalysisManager()->getGrammarInterface()->getConstructsByType(
					InstRO::Tooling::GrammarInterface::GTFunction);

	output = std::make_unique<InstRO::Core::ConstructSet>(csFunctions->intersect(*(csByNameMain.get())));
}
Core::ConstructSet* ProgramEntrySelector::getOutput() { return output.get(); }
void ProgramEntrySelector::releaseOutput() { output->clear(); }

}	// Selector
}	// InstRO