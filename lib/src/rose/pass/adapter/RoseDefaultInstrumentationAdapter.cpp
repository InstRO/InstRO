#include "instro/rose/pass/adapter/RoseDefaultInstrumentationAdapter.h"
#include "instro/utility/Logger.h"

namespace InstRO {
namespace Rose {
namespace Adapter {

void RoseDefaultInstrumentationAdapter::insertRuntimeStartup() {
	// TODO implement me
	logIt(ERROR) << "RoseDefaultinstrumentationAdapter: insertRuntimeStartup() not implemented." << std::endl;
}

void RoseDefaultInstrumentationAdapter::instrumentFunction(const std::shared_ptr<InstRO::Core::Construct> construct) {
	// TODO implement me
	logIt(ERROR) << "RoseDefaultinstrumentationAdapter: instrumentFunction() not implemented." << std::endl;
}

void RoseDefaultInstrumentationAdapter::instrumentLoop(const std::shared_ptr<InstRO::Core::Construct> construct) {
	// TODO implement me
	logIt(ERROR) << "RoseDefaultinstrumentationAdapter: instrumentLoop() not implemented." << std::endl;
}

void RoseDefaultInstrumentationAdapter::instrumentConditional(const std::shared_ptr<InstRO::Core::Construct> construct) {
	// TODO implement me
	logIt(ERROR) << "RoseDefaultinstrumentationAdapter: instrumentConditional() not implemented." << std::endl;
}

void RoseDefaultInstrumentationAdapter::instrumentScope(const std::shared_ptr<InstRO::Core::Construct> construct) {
	// TODO implement me
	logIt(ERROR) << "RoseDefaultinstrumentationAdapter: instrumentScope() not implemented." << std::endl;
}

void RoseDefaultInstrumentationAdapter::instrumentStatement(const std::shared_ptr<InstRO::Core::Construct> construct) {
	// TODO implement me
	logIt(ERROR) << "RoseDefaultinstrumentationAdapter: instrumentStatement() not implemented." << std::endl;
}

void RoseDefaultInstrumentationAdapter::instrumentExpression(const std::shared_ptr<InstRO::Core::Construct> construct) {
	// TODO implement me
	logIt(ERROR) << "RoseDefaultinstrumentationAdapter: instrumentExpression() not implemented." << std::endl;
}

}	// namespace Adapter
}	// namespace Rose
}	// namespace InstRO
