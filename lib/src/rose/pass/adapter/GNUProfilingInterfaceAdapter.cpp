#include "instro/core/Pass.h"
#include "instro/core/ConstructSet.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/pass/adapter/GNUProfilingInterfaceAdapter.h"

namespace InstRO {
namespace Rose {
namespace Adapter {

void GNUProfilingInterfaceAdapter::execute() {
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface cs(inputPass->getOutput());

	// TODO implement me
}

}	// InstRO
}	// Rose
}	// InstRO
