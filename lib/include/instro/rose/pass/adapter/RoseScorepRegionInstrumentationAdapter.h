#ifndef INSTRO_ROSE_PASS_ADAPTER_SCOREP_REGION_ADAPTER_H
#define INSTRO_ROSE_PASS_ADAPTER_SCOREP_REGION_ADAPTER_H

#include "instro/rose/pass/adapter/support/RosePostOrderInstrumentationAdapter.h"
#include "instro/rose/pass/adapter/support/RoseScorepCodeWrapper.h"
#include "rose.h"

namespace InstRO {
namespace Rose {
namespace Adapter {

/**
 * Uses ScoreP's user regions mechanism to insert instrumentation.
 *
 * \author JP Lehr
 */
class RoseScorepRegionInstrumentationAdapter : public Support::RosePostOrderInstrumentationAdapter {
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
	Support::RoseScorepCodeWrapper wrapper;
};
}
}
}

#endif
