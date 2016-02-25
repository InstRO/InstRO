#include "instro/rose/pass/adapter/RoseDefaultInstrumentationAdapter.h"
#include "instro/tooling/IdentifierProvider.h"
#include "instro/utility/Logger.h"

#include <map>

namespace InstRO {
namespace Rose {
namespace Adapter {

void RoseDefaultInstrumentationAdapter::instrumentFunction(const std::shared_ptr<InstRO::Core::Construct> construct) {
	auto funcDef = isSgFunctionDefinition(InstRO::Rose::toRoseConstruct(construct)->getNode());
	auto id = InstRO::Tooling::IdentifierProvider::getID(construct);

	wrapper.instrumentFunction(funcDef, id);
}

void RoseDefaultInstrumentationAdapter::instrumentLoop(const std::shared_ptr<InstRO::Core::Construct> construct) {
	instrumentAsStatement(construct, std::string("loop"));
}

void RoseDefaultInstrumentationAdapter::instrumentConditional(
		const std::shared_ptr<InstRO::Core::Construct> construct) {
	instrumentAsStatement(construct, std::string("conditional"));
}

void RoseDefaultInstrumentationAdapter::instrumentScope(const std::shared_ptr<InstRO::Core::Construct> construct) {
	auto scope = isSgScopeStatement(InstRO::Rose::toRoseConstruct(construct)->getNode());
	auto id = InstRO::Tooling::IdentifierProvider::getID(construct);

	wrapper.instrumentScope(scope, id);
}

void RoseDefaultInstrumentationAdapter::instrumentStatement(const std::shared_ptr<InstRO::Core::Construct> construct) {
	instrumentAsStatement(construct, std::string("simple_statement"));
}

void RoseDefaultInstrumentationAdapter::instrumentExpression(const std::shared_ptr<InstRO::Core::Construct> construct) {
	auto expr = isSgExpression(InstRO::Rose::toRoseConstruct(construct)->getNode());
	auto id = InstRO::Tooling::IdentifierProvider::getID(construct);

	wrapper.wrapExpression(expr, id);
}

void RoseDefaultInstrumentationAdapter::instrumentAsStatement(const std::shared_ptr<InstRO::Core::Construct> construct,
																															std::string namePostfix) {
	auto stmt = isSgStatement(InstRO::Rose::toRoseConstruct(construct)->getNode());
	auto id = InstRO::Tooling::IdentifierProvider::getID(construct);

	wrapper.wrapStatement(stmt, namePostfix, id);
}


void RoseDefaultInstrumentationAdapter::visit(SgNode *astNode) {
	if (sgNodesToInstrument.find(astNode) != sgNodesToInstrument.end()) {
		auto constructToInstrument = sgNodesToInstrument[astNode];
		sgNodesToInstrument.erase(astNode);	// erase so it is not instrumented twice
		instrument(constructToInstrument);
	}
}

void RoseDefaultInstrumentationAdapter::execute() {
	InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface csci(getInput(0));
	for (auto construct : csci) {
		auto roseConstruct = InstRO::Rose::toRoseConstruct(construct);
		sgNodesToInstrument[roseConstruct->getNode()] = construct;
	}

	this->traverse(project, Order::postorder);
	// After the instrumentation is done, we right out an nm file providing the human readable identifiers	
	InstRO::Tooling::IdentifierProvider::exportToNMFile(InstRO::Tooling::IdentifierProvider::getIdentifierMap(getInput(0)));
}

using InstRO::Core::ConstructTraitType;
void RoseDefaultInstrumentationAdapter::instrument(std::shared_ptr<InstRO::Core::Construct> construct) {
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
		return;
	}

	else if (ct.is(ConstructTraitType::CTScopeStatement)) {
		instrumentScope(construct);
	}

	else if (!ct.is(ConstructTraitType::CTWrappableStatement)) {
		// TODO 2015-11 RN: actually we should create an enclosing scope in this case
		logIt(WARN) << "DefaultInstrumentationAdapter encountered unwrappable Statement" << std::endl
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

}	// namespace Adapter
}	// namespace Rose
}	// namespace InstRO
