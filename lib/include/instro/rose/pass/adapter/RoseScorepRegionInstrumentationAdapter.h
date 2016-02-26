#ifndef INSTRO_ROSE_PASS_ADAPTER_SCOREP_REGION_ADAPTER_H
#define INSTRO_ROSE_PASS_ADAPTER_SCOREP_REGION_ADAPTER_H

#include "instro/rose/pass/adapter/RoseDefaultInstrumentationAdapter.h"
#include "instro/rose/pass/adapter/support/RoseCodeWrapper.h"
#include "rose.h"

namespace InstRO {
namespace Rose {
namespace Adapter {

class RoseScorepRegionInstrumentationAdapter : public InstRO::Rose::Adapter::RosePostOrderInstrumentationAdapter {
 public:
	RoseScorepRegionInstrumentationAdapter(SgProject *p) : RosePostOrderInstrumentationAdapter(p), wrapper(p) {}

 protected:
	void instrumentFunction(const std::shared_ptr<InstRO::Core::Construct> construct) override;
	void instrumentLoop(const std::shared_ptr<InstRO::Core::Construct> construct) override;
	void instrumentConditional(const std::shared_ptr<InstRO::Core::Construct> construct) override;
	void instrumentScope(const std::shared_ptr<InstRO::Core::Construct> construct) override;
	void instrumentStatement(const std::shared_ptr<InstRO::Core::Construct> construct) override;
	void instrumentExpression(const std::shared_ptr<InstRO::Core::Construct> construct) override;

 private:
	Support::RoseArbitraryTextCodeWrapper wrapper;
};
}
}
}

#endif
