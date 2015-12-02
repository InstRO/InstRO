#ifndef INSTRO_ADATPER_CONSTRUCT_PRINTER_ADAPTER_H
#define INSTRO_ADATPER_CONSTRUCT_PRINTER_ADAPTER_H

#include "instro/core/Pass.h"
#include "instro/rose/core/RosePassImplementation.h"

namespace InstRO {
namespace Adapter {

class ConstructPrinterAdapter : public InstRO::Core::PassImplementation {
 protected:
	InstRO::Pass *inputPass;

 public:
	ConstructPrinterAdapter(InstRO::Pass *pass)
			: PassImplementation(InstRO::Core::ChannelConfiguration(pass)), inputPass(pass) {}
	void execute() override;
};
}
}

#endif
