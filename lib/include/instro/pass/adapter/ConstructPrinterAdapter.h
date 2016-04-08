#ifndef INSTRO_ADATPER_CONSTRUCT_PRINTER_ADAPTER_H
#define INSTRO_ADATPER_CONSTRUCT_PRINTER_ADAPTER_H

#include "instro/core/Pass.h"
#include "instro/rose/core/RosePassImplementation.h"

namespace InstRO {
namespace Adapter {

class ConstructPrinterAdapter : public InstRO::Core::PassImplementation {
 public:
	ConstructPrinterAdapter() : PassImplementation() {}
	void execute() override;
};
}
}

#endif
