#include "instro/pass/selector/ProgramEntrySelector.h"

#include "instro/core/Instrumentor.h"
#include "instro/core/Singleton.h"
#include "instro/tooling/ConstructClassInterface.h"
#include "instro/tooling/NamedConstructAccess.h"

#include <string>
#include <vector>

namespace InstRO {
namespace Selector {

void ProgramEntrySelector::execute() {
	std::vector<std::string> mainString = { std::string("main"), std::string("::main")};
	InstRO::Tooling::NamedConstructAccess::WildcardedStringMatcher mainMatcher(mainString);

	auto analysisManager = getInstrumentorInstance()->getAnalysisManager();

	InstRO::Core::ConstructSet csByNameMain =
			analysisManager->getNamedConstructAccessFacility()->getConstructsByIdentifierName(mainMatcher);

	auto allFunctions = analysisManager->getGrammarInterface()->getConstructsByClass(
			InstRO::Core::ConstructTraitType::CTFunction);

	// intersect everything called "*main" with all functions
	outputSet = allFunctions.intersect(csByNameMain);
}

}	// Selector
}	// InstRO
