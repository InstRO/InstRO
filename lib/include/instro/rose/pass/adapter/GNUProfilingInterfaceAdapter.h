#ifndef INSTRO_ROSE_RoseConstructPrinter
#define INSTRO_ROSE_RoseConstructPrinter
#include "instro/core/Pass.h"
#include "instro/core/ConstructSet.h"
#include "instro/rose/core/RosePassImplementation.h"

namespace InstRO {
namespace Rose {
namespace Adapter {

class GNUProfilingInterfaceAdapter : public RosePassImplementation {
 protected:
	::InstRO::Pass *inputPass;
	::InstRO::Core::ConstructSet outputCS;

 public:
	// This Pass only accepts functions
	GNUProfilingInterfaceAdapter(::InstRO::Pass *pass)
			: RosePassImplementation(::InstRO::Core::ChannelConfiguration(
						{pass, ::InstRO::Core::ConstructTraitType::CTFunction, ::InstRO::Core::ConstructTraitType::CTFunction})),
				inputPass(pass) {}
	virtual void init(){};
	virtual void execute() override;
	virtual void finalize(){};
	virtual void releaseOutput() { outputCS.clear(); };
	virtual InstRO::Core::ConstructSet *getOutput() { return &outputCS; }
};
}
}
}
#endif
