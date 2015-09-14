#include <memory>
#include "instro/core/Helper.h"
#include "instro/core/ConstructSet.h"
#include "instro/example/CompoundSelector.h"

namespace InstRO {
namespace Example {
namespace Selectors {
namespace CompoundSelectorHelper {
std::unique_ptr<InstRO::Core::ConstructSet> strictCompareOr(InstRO::Core::ConstructSet* a,
																														InstRO::Core::ConstructSet* b) {
	//	std::unique_ptr<InstRO::Core::ConstructSet> retSet = std::make_unique<InstRO::Core::ConstructSet>();
	InstRO::Core::ConstructSet cs = b->combine(*a);
	std::unique_ptr<InstRO::Core::ConstructSet> retSet = std::make_unique<InstRO::Core::ConstructSet>(cs);
	return retSet;
}
std::unique_ptr<InstRO::Core::ConstructSet> strictCompareAnd(InstRO::Core::ConstructSet* a,
																														 InstRO::Core::ConstructSet* b) {
	//	std::unique_ptr<InstRO::Core::ConstructSet> retSet = std::make_unique<InstRO::Core::ConstructSet>();
	std::unique_ptr<InstRO::Core::ConstructSet> retSet = std::make_unique<InstRO::Core::ConstructSet>(b->intersect(*a));
	return retSet;
}
std::unique_ptr<InstRO::Core::ConstructSet> parentChildCompareAnd(InstRO::Core::ConstructSet* a,
																																	InstRO::Core::ConstructSet* b, bool keepParent,
																																	bool keepChild) {
	//	std::unique_ptr<InstRO::Core::ConstructSet> retSet = std::make_unique<InstRO::Core::ConstructSet>();
	std::unique_ptr<InstRO::Core::ConstructSet> retSet = std::make_unique<InstRO::Core::ConstructSet>(b->intersect(*a));
	return retSet;
}
}

void CompoundSelector::execute() {
	// This will heavily rely on construct elevation ...
	switch (getCompoundOperationType()) {
		case CompoundOperationType::CO_Or:
			passOutput = CompoundSelectorHelper::strictCompareOr(firstArg->getOutput(), secondArg->getOutput());
			break;
		case CompoundOperationType::CO_And:
			if (isStrict())
				passOutput = CompoundSelectorHelper::strictCompareAnd(firstArg->getOutput(), secondArg->getOutput());
			if (isParentChild())
				passOutput =
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
