#include "instro/pass/adapter/DefaultInstrumentationAdapter.h"

#include "instro/utility/Logger.h"

namespace InstRO {
namespace Adapter {

using InstRO::Core::ConstructTraitType;

void DefaultInstrumentationAdapter::execute() {

	InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface csci(inputCS);
	for (auto construct : csci) {
		auto ct = construct->getTraits();

		if (ct.is(ConstructTraitType::CTFunction)) {
			instrumentFunction(construct);
		}

		else if (ct.is(ConstructTraitType::CTExpression)) {
			instrumentExpression(construct);
		}

		else if (!ct.is(ConstructTraitType::CTStatement)) {
			logIt(WARN) << "DefaultInstrumentationAdapter encountered incompatible Construct" << std::endl
									<< "\t" << construct->toString() << std::endl;
		}

		else if (ct.is(ConstructTraitType::CTScopeStatement)) {
			instrumentScope(construct);
		}

		else if (!ct.is(ConstructTraitType::CTWrappableStatement)) {
			// TODO 2015-11 RN: actually we should create an enclosing scope in this case
			logIt(WARN) << "DefaultInstrumentationAdapter encountered unwrappable Statement" << std::endl
									<< "\t" << construct->toString() << std::endl;
		}

		else if (ct.is(ConstructTraitType::CTLoopStatement)) {
			instrumentLoop(construct);
		}

		else if (ct.is(ConstructTraitType::CTConditionalStatement)) {
			instrumentConditional(construct);
		}


		else if (ct.is(ConstructTraitType::CTSimpleStatement)) {
			instrumentStatement(construct);
		}
	}
}

}	// namespace Adapter
}	// namespace InstRO
