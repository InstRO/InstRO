#ifndef INSTRO_ROSE_RoseConstructPrinter
#define INSTRO_ROSE_RoseConstructPrinter
#include "instro/core/Pass.h"
#include "instro/rose/core/RosePassImplementation.h"

namespace InstRO {
namespace Rose {
namespace Adapter {

class RoseConstructPrinter : public RosePassImplementation {
 protected:
	InstRO::Pass *inputPass;
	InstRO::Core::ConstructSet outputCS;

 public:
	RoseConstructPrinter(InstRO::Pass *pass)
			: RosePassImplementation(InstRO::Core::ChannelConfiguration(pass)), inputPass(pass) {}
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
