#ifndef INSTRO_ROSE_PASS_ADAPTER_ROSEDEFAULTINSTRUMENTATIONADAPTER_H_
#define INSTRO_ROSE_PASS_ADAPTER_ROSEDEFAULTINSTRUMENTATIONADAPTER_H_

#include "instro/pass/adapter/DefaultInstrumentationAdapter.h"
#include "instro/rose/pass/adapter/support/RoseCodeWrapper.h"
#include "instro/rose/core/RoseConstructSet.h"

#include <rose.h>

namespace InstRO {
namespace Rose {
namespace Adapter {

/**
 * \author Roman Ness
 */
class RoseDefaultInstrumentationAdapter : public InstRO::Adapter::DefaultInstrumentationAdapter,
																					public AstSimpleProcessing {
 public:
	RoseDefaultInstrumentationAdapter(SgProject* project)
			: DefaultInstrumentationAdapter(), wrapper(project), project(project) {}
	virtual ~RoseDefaultInstrumentationAdapter() {}

	void execute() override;

 protected:
	void instrumentFunction(const std::shared_ptr<InstRO::Core::Construct> construct) override;
	void instrumentLoop(const std::shared_ptr<InstRO::Core::Construct> construct) override;
	void instrumentConditional(const std::shared_ptr<InstRO::Core::Construct> construct) override;
	void instrumentScope(const std::shared_ptr<InstRO::Core::Construct> construct) override;
	void instrumentStatement(const std::shared_ptr<InstRO::Core::Construct> construct) override;
	void instrumentExpression(const std::shared_ptr<InstRO::Core::Construct> construct) override;

	void visit(SgNode* astNode) ;
	void instrument(std::shared_ptr<InstRO::Core::Construct> construct);

 private:
	Support::RoseCodeWrapper wrapper;
	SgProject* project;

	std::map<SgNode*, std::shared_ptr<InstRO::Core::Construct> > sgNodesToInstrument;

	void instrumentAsStatement(const std::shared_ptr<InstRO::Core::Construct> construct, std::string namePostfix);
};

}	// namespace Adapter
}	// namespace Rose
}	// namespace InstRO

#endif /* INSTRO_ROSE_PASS_ADAPTER_ROSEDEFAULTINSTRUMENTATIONADAPTER_H_ */
