#include "instro/pass/adapter/ConstructPrinterAdapter.h"

#include "instro/core/ConstructSet.h"

#include <iostream>
#include <iomanip>	// setw

namespace InstRO {
namespace Adapter {

void ConstructPrinterAdapter::execute() {
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface cs(inputPass->getOutput());

	auto minLevel = inputPass->getOutput()->getMinConstructLevel();
	auto maxLevel = inputPass->getOutput()->getMaxConstructLevel();

	std::cout << "=ConstructPrinter= ConstructSet contains " << cs.size() << " constructs";
	if (!cs.empty()) {
		std::cout << " | min: " << InstRO::Core::constructLevelToString(minLevel)
							<< " | max: " << InstRO::Core::constructLevelToString(maxLevel) << std::endl;
	} else {
		std::cout << std::endl;
	}

	unsigned long count = 0;
	for (auto construct : cs) {
		std::cout << std::setw(4) << count << ": " << construct->getTraits().toStringShort()
							<< ":  " << construct->toString() << std::endl;
		count++;
	}
}

}	// namespace Adapter
}	// namespace InstRO
