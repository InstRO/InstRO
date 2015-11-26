#include "instro/core/Instrumentor.h"
#include "instro/tooling/NamedConstructAccess.h"
#include "instro/pass/selector/IdentifyerSelector.h"
#include "instro/tooling/AnalysisInterface.h"
#include "instro/core/Singleton.h"

namespace InstRO {
namespace Selector {

void IdentifyerSelector::execute() {
	Tooling::NamedConstructAccess::NamedConstructAccess *nca =
			getInstrumentorInstance()->getAnalysisManager()->getNamedConstructAccessFacility();

	Tooling::NamedConstructAccess::WildcardedStringMatcher stringMatcher(rules);
	
	outputSet = nca->getConstructsByIdentifierName(stringMatcher);
}

}
}
