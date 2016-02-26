#include "instro/rose/pass/adapter/RoseScorepRegionInstrumentationAdapter.h"
#include "instro/utility/Logger.h"

void InstRO::Rose::Adapter::RoseScorepRegionInstrumentationAdapter::instrumentFunction(
		const std::shared_ptr<InstRO::Core::Construct> construct) {
	logIt(DEBUG) << "ScorepRegionAdapter instrumentFunction" << std::endl;
	throw std::string("not implemented");
}

void InstRO::Rose::Adapter::RoseScorepRegionInstrumentationAdapter::instrumentLoop(
		const std::shared_ptr<InstRO::Core::Construct> construct) {
	logIt(DEBUG) << "ScorepRegionAdapter instrumentLoop" << std::endl;
	throw std::string("not implemented");
}

void InstRO::Rose::Adapter::RoseScorepRegionInstrumentationAdapter::instrumentConditional(
		const std::shared_ptr<InstRO::Core::Construct> construct) {
	logIt(DEBUG) << "ScorepRegionAdapter instrumentConditional" << std::endl;
	throw std::string("not implemented");
}

void InstRO::Rose::Adapter::RoseScorepRegionInstrumentationAdapter::instrumentScope(
		const std::shared_ptr<InstRO::Core::Construct> construct) {
	logIt(DEBUG) << "ScorepRegionAdapter instrumentScope" << std::endl;
	throw std::string("not implemented");
}

void InstRO::Rose::Adapter::RoseScorepRegionInstrumentationAdapter::instrumentStatement(
		const std::shared_ptr<InstRO::Core::Construct> construct) {
	logIt(DEBUG) << "ScorepRegionAdapter instrumentStatement" << std::endl;
	throw std::string("not implemented");
}

void InstRO::Rose::Adapter::RoseScorepRegionInstrumentationAdapter::instrumentExpression(
		const std::shared_ptr<InstRO::Core::Construct> construct) {
	logIt(DEBUG) << "ScorepRegionAdapter instrumentExpression" << std::endl;
	throw std::string("not implemented");
}
