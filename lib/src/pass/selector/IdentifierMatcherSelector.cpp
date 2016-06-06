#include "instro/pass/selector/IdentifierMatcherSelector.h"

#include "instro/Instrumentor.h"
#include "instro/tooling/NamedConstructAccess.h"
#include "instro/tooling/AnalysisInterface.h"
#include "instro/core/Singleton.h"

namespace InstRO {
namespace Selector {

void IdentifierMatcherSelector::execute() {
	Tooling::NamedConstructAccess::NamedConstructAccess *nca =
			getInstrumentorInstance()->getAnalysisManager()->getNamedConstructAccessFacility();

	Tooling::NamedConstructAccess::WildcardedStringMatcher stringMatcher(rules);

	outputSet = nca->getConstructsByIdentifierName(stringMatcher);
}
}
}
