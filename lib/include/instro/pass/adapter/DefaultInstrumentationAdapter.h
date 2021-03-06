#ifndef LIB_INCLUDE_INSTRO_PASS_ADAPTER_DEFAULTINSTRUMENTATIONADAPTER_H_
#define LIB_INCLUDE_INSTRO_PASS_ADAPTER_DEFAULTINSTRUMENTATIONADAPTER_H_

#include "instro/core/PassImplementation.h"
#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Adapter {

/**
 * \brief Instruments incoming Constructs with the InstRO Measurement Interface.
 * \author Roman Ness
 */
class DefaultInstrumentationAdapter : public InstRO::Core::PassImplementation {
 public:
	DefaultInstrumentationAdapter() : PassImplementation() {}
	virtual ~DefaultInstrumentationAdapter() {}

 protected:
	virtual void instrumentFunction(const std::shared_ptr<InstRO::Core::Construct> construct) = 0;
	virtual void instrumentLoop(const std::shared_ptr<InstRO::Core::Construct> construct) = 0;
	virtual void instrumentConditional(const std::shared_ptr<InstRO::Core::Construct> construct) = 0;
	virtual void instrumentScope(const std::shared_ptr<InstRO::Core::Construct> construct) = 0;
	virtual void instrumentStatement(const std::shared_ptr<InstRO::Core::Construct> construct) = 0;
	virtual void instrumentExpression(const std::shared_ptr<InstRO::Core::Construct> construct) = 0;
};

}	// namespace Adapter
}	// namespace InstRO

#endif /* LIB_INCLUDE_INSTRO_PASS_ADAPTER_DEFAULTINSTRUMENTATIONADAPTER_H_ */
