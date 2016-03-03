#include "instro/rose/pass/adapter/RoseDefaultInstrumentationAdapter.h"
#include "instro/tooling/IdentifierProvider.h"
#include "instro/utility/Logger.h"

#include <map>

namespace InstRO {
namespace Rose {
namespace Adapter {
namespace Support {

void RosePostOrderInstrumentationAdapter::visit(SgNode *astNode) {
	if (sgNodesToInstrument.find(astNode) != sgNodesToInstrument.end()) {
		auto constructToInstrument = sgNodesToInstrument[astNode];
		sgNodesToInstrument.erase(astNode);	// erase so it is not instrumented twice
		instrument(constructToInstrument);
	}
}

void RosePostOrderInstrumentationAdapter::execute() {
	InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface csci(getInput(0));
	for (auto construct : csci) {
		auto roseConstruct = InstRO::Rose::toRoseConstruct(construct);
		sgNodesToInstrument[roseConstruct->getNode()] = construct;
	}

	this->traverse(project, Order::postorder);
	// After the instrumentation is done, we right out an nm file providing the human readable identifiers
	InstRO::Tooling::IdentifierProvider::exportToNMFile(
			InstRO::Tooling::IdentifierProvider::getIdentifierMap(getInput(0)));
}

void RosePostOrderInstrumentationAdapter::instrument(std::shared_ptr<InstRO::Core::Construct> construct) {
	using InstRO::Core::ConstructTraitType;
	auto ct = construct->getTraits();

	if (ct.is(ConstructTraitType::CTFunction)) {
		instrumentFunction(construct);
	}

	else if (ct.is(ConstructTraitType::CTExpression)) {
		instrumentExpression(construct);
	}

	else if (!ct.is(ConstructTraitType::CTStatement)) {
		logIt(WARN) << "PostOrderInstrumentationAdapter encountered incompatible Construct" << std::endl
								<< "\t" << construct->toString() << std::endl;
		return;
	}

	else if (ct.is(ConstructTraitType::CTScopeStatement)) {
		instrumentScope(construct);
	}

	else if (!ct.is(ConstructTraitType::CTWrappableStatement)) {
		// TODO 2015-11 RN: actually we should create an enclosing scope in this case
		logIt(WARN) << "PostOrderInstrumentationAdapter encountered unwrappable Statement" << std::endl
								<< "\t" << construct->toString() << std::endl;
		return;
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
}
}
}
