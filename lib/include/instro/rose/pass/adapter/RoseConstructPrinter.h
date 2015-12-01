#ifndef INSTRO_ROSE_ROSE_CONSTRUCT_PRINTER_H
#define INSTRO_ROSE_ROSE_CONSTRUCT_PRINTER_H

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
	void execute() override;
};
}
}
}
#endif
