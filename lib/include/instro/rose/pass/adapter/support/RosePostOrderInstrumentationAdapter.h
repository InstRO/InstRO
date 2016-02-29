#ifndef INSTRO_ROSE_PASS_ADAPTER_SUPPORT_ROSE_POST_ORDER_INSTRUMENTATION_ADAPTER_H
#define INSTRO_ROSE_PASS_ADAPTER_SUPPORT_ROSE_POST_ORDER_INSTRUMENTATION_ADAPTER_H

#include "instro/rose/core/RoseConstructSet.h"
#include "instro/pass/adapter/DefaultInstrumentationAdapter.h"

#include "rose.h"

namespace InstRO {
namespace Rose {
namespace Adapter {
namespace Support {

/**
 * Provides functionality of invoking a post order visit on the Ast and dispatching the constructs which need to be
 * instrumented to the respective functions inherited from DefaultInstrumentationAdapter.
 *
 * \author Roman Ness, JP Lehr
 */
class RosePostOrderInstrumentationAdapter : public InstRO::Adapter::DefaultInstrumentationAdapter,
																						public AstSimpleProcessing {
 public:
	RosePostOrderInstrumentationAdapter(SgProject* p) : DefaultInstrumentationAdapter(), project(p) {}
	virtual ~RosePostOrderInstrumentationAdapter() {}

	void execute() override;

 protected:
	void visit(SgNode* astNode);
	void instrument(std::shared_ptr<InstRO::Core::Construct> construct);

	SgProject* project;
	std::map<SgNode*, std::shared_ptr<InstRO::Core::Construct> > sgNodesToInstrument;
};

} // Support
}	// Adapter
}	// Rose
}	// InstRO

#endif
