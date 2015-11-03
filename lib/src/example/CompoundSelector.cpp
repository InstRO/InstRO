#include <memory>
#include "instro/core/Helper.h"
#include "instro/core/ConstructSet.h"
#include "instro/example/CompoundSelector.h"

namespace InstRO {
namespace Example {
namespace Selectors {
namespace CompoundSelectorHelper {

InstRO::Core::ConstructSet strictCompareOr(InstRO::Core::ConstructSet* a, InstRO::Core::ConstructSet* b) {
	return b->combine(*a);
}
InstRO::Core::ConstructSet strictCompareAnd(InstRO::Core::ConstructSet* a, InstRO::Core::ConstructSet* b) {
	return b->intersect(*a);
}
InstRO::Core::ConstructSet parentChildCompareAnd(InstRO::Core::ConstructSet* a, InstRO::Core::ConstructSet* b,
																								 bool keepParent, bool keepChild) {
	return b->intersect(*a);
}

}

void CompoundSelector::execute() {
	// This will heavily rely on construct elevation ...
	switch (getCompoundOperationType()) {
		case CompoundOperationType::CO_Or:
			outputSet = CompoundSelectorHelper::strictCompareOr(firstArg->getOutput(), secondArg->getOutput());
			break;
		case CompoundOperationType::CO_And:
			if (isStrict())
				outputSet = CompoundSelectorHelper::strictCompareAnd(firstArg->getOutput(), secondArg->getOutput());
			if (isParentChild())
				outputSet =
						CompoundSelectorHelper::parentChildCompareAnd(firstArg->getOutput(), secondArg->getOutput(), true, false);
			break;
		case CompoundOperationType::CO_XOr:
			break;
		default:
			break;
	}
}
}
}
}
