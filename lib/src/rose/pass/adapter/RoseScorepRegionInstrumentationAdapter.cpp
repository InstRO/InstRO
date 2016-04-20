#include "instro/rose/pass/adapter/RoseScorepRegionInstrumentationAdapter.h"
#include "instro/tooling/IdentifierProvider.h"

#include "instro/utility/Logger.h"

void InstRO::Rose::Adapter::RoseScorepRegionInstrumentationAdapter::instrumentFunction(
		const std::shared_ptr<InstRO::Core::Construct> construct) {
	auto funcDef = isSgFunctionDefinition(InstRO::Rose::toRoseConstruct(construct)->getNode());
	auto identifier = InstRO::Tooling::IdentifierProvider::getIdentifier(construct);

	wrapper.instrumentFunction(funcDef, identifier);
}

void InstRO::Rose::Adapter::RoseScorepRegionInstrumentationAdapter::instrumentLoop(
		const std::shared_ptr<InstRO::Core::Construct> construct) {
	auto loop = isSgLocatedNode(InstRO::Rose::toRoseConstruct(construct)->getNode());
	auto identifier = InstRO::Tooling::IdentifierProvider::getIdentifier(construct);

	wrapper.wrapStatement(loop, identifier);
}

void InstRO::Rose::Adapter::RoseScorepRegionInstrumentationAdapter::instrumentConditional(
		const std::shared_ptr<InstRO::Core::Construct> construct) {
	auto conditional = isSgLocatedNode(InstRO::Rose::toRoseConstruct(construct)->getNode());
	auto identifier = InstRO::Tooling::IdentifierProvider::getIdentifier(construct);

	wrapper.wrapStatement(conditional, identifier);
}

void InstRO::Rose::Adapter::RoseScorepRegionInstrumentationAdapter::instrumentScope(
		const std::shared_ptr<InstRO::Core::Construct> construct) {
	auto scope = isSgScopeStatement(InstRO::Rose::toRoseConstruct(construct)->getNode());
	auto identifier = InstRO::Tooling::IdentifierProvider::getIdentifier(construct);

	wrapper.instrumentScope(scope, identifier);
}

void InstRO::Rose::Adapter::RoseScorepRegionInstrumentationAdapter::instrumentStatement(
		const std::shared_ptr<InstRO::Core::Construct> construct) {
	auto stmt = isSgLocatedNode(InstRO::Rose::toRoseConstruct(construct)->getNode());
	auto identifier = InstRO::Tooling::IdentifierProvider::getIdentifier(construct);

	wrapper.wrapStatement(stmt, identifier);
}

void InstRO::Rose::Adapter::RoseScorepRegionInstrumentationAdapter::instrumentExpression(
		const std::shared_ptr<InstRO::Core::Construct> construct) {
	logIt(ERROR) << "ScorepRegionAdapter instrumentExpression not implemented" << std::endl;
}
