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

 public:
	RoseConstructPrinter(InstRO::Pass *pass)
			: RosePassImplementation(InstRO::Core::ChannelConfiguration(pass)), inputPass(pass) {}
	void init(){};
	void execute() override;
	void finalize(){};
};
}
}
}
#endif
