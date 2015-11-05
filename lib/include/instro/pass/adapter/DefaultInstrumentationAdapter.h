#ifndef LIB_INCLUDE_INSTRO_PASS_ADAPTER_DEFAULTINSTRUMENTATIONADAPTER_H_
#define LIB_INCLUDE_INSTRO_PASS_ADAPTER_DEFAULTINSTRUMENTATIONADAPTER_H_

#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Adapter {

/**
 * \brief Instruments incoming Constructs with the InstRO Measurement Interface.
 * \author Roman Ness
 * TODO implement me
 */
class DefaultInstrumentationAdapter : public InstRO::Core::PassImplementation {
public:
	DefaultInstrumentationAdapter(InstRO::Pass* input) :
			PassImplementation(InstRO::Core::ChannelConfiguration(input)) {}

	void execute() override;

};

}	// namespace Adapter
}	// namespace InstRO

#endif /* LIB_INCLUDE_INSTRO_PASS_ADAPTER_DEFAULTINSTRUMENTATIONADAPTER_H_ */
