#include "instro/core/Instrumentor.h"
#include "instro/core/Singleton.h"
#include "instro/tooling/GrammarInterface.h"
#include "instro/tooling/NamedConstructAccess.h"
#include "instro/pass/selector/ProgramEntrySelector.h"

#include <string>
#include <list>

namespace InstRO {
namespace Selector {
void ProgramEntrySelector::init() {
}

void ProgramEntrySelector::finalize() {
}

void ProgramEntrySelector::execute() {
	std::list<std::string> mainString = { std::string("main") };
	auto mainMatcher = InstRO::Tooling::NamedConstructAccess::WildcardedStringMatcher(mainString);

	InstRO::Core::ConstructSet csByNameMain =
			getInstrumentorInstance()->getAnalysisManager()->getNamedConstructAccessFacility()->getConstructsByIdentifyerName(
					mainMatcher);

	// we now have anything that is called main. intersect it with the set of function statements
	auto csFunctions = getInstrumentorInstance()->getAnalysisManager()->getGrammarInterface()->getConstructsByClass(
			InstRO::Core::ConstructTraitType::CTFunction);

	outputSet = csFunctions.intersect(csByNameMain);
}

}	// Selector
}	// InstRO
