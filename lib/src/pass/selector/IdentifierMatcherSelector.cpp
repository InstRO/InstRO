#include "instro/pass/selector/IdentifierMatcherSelector.h"

#include "instro/Instrumentor.h"
#include "instro/core/Singleton.h"
#include "instro/tooling/AnalysisInterface.h"
#include "instro/tooling/NamedConstructAccess.h"

namespace InstRO {
namespace Selector {

void IdentifierMatcherSelector::execute() {
	Tooling::NamedConstructAccess::NamedConstructAccess* nca =
			getInstrumentorInstance()->getAnalysisManager()->getNamedConstructAccessFacility();

	Tooling::NamedConstructAccess::WildcardedStringMatcher stringMatcher(rules);

	outputSet = nca->getConstructsByIdentifierName(stringMatcher);
}
}	// namespace Selector
}	// namespace InstRO
