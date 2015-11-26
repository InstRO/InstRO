#include "instro/rose/pass/adapter/RoseDefaultInstrumentationAdapter.h"
#include "instro/tooling/IdentifierProvider.h"
#include "instro/utility/Logger.h"

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

void RoseDefaultInstrumentationAdapter::instrumentAsStatement(
		const std::shared_ptr<InstRO::Core::Construct> construct, std::string namePostfix) {

	auto stmt = isSgStatement(InstRO::Rose::toRoseConstruct(construct)->getNode());
	auto id = InstRO::Tooling::IdentifierProvider::getID(construct);

	wrapper.wrapStatement(stmt, namePostfix, id);
}

}	// namespace Adapter
}	// namespace Rose
}	// namespace InstRO
