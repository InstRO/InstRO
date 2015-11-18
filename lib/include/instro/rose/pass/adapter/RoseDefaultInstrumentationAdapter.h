#ifndef INSTRO_ROSE_PASS_ADAPTER_ROSEDEFAULTINSTRUMENTATIONADAPTER_H_
#define INSTRO_ROSE_PASS_ADAPTER_ROSEDEFAULTINSTRUMENTATIONADAPTER_H_

#include "instro/pass/adapter/DefaultInstrumentationAdapter.h"

namespace InstRO {
namespace Rose {
namespace Adapter {

/**
 * \author Roman Ness
 */
class RoseDefaultInstrumentationAdapter : public InstRO::Adapter::DefaultInstrumentationAdapter {
 public:
	RoseDefaultInstrumentationAdapter(InstRO::Pass* input) : DefaultInstrumentationAdapter(input) {}

 protected:
	void insertRuntimeStartup();

	void instrumentFunction(const std::shared_ptr<InstRO::Core::Construct> construct);
	void instrumentLoop(const std::shared_ptr<InstRO::Core::Construct> construct);
	void instrumentConditional(const std::shared_ptr<InstRO::Core::Construct> construct);
	void instrumentScope(const std::shared_ptr<InstRO::Core::Construct> construct);
	void instrumentStatement(const std::shared_ptr<InstRO::Core::Construct> construct);
	void instrumentExpression(const std::shared_ptr<InstRO::Core::Construct> construct);
};

}	// namespace Adapter
}	// namespace Rose
}	// namespace InstRO

#endif /* INSTRO_ROSE_PASS_ADAPTER_ROSEDEFAULTINSTRUMENTATIONADAPTER_H_ */
