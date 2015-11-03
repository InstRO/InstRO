#include <memory>
#include "instro/core/Pass.h"
#include "instro/example/ExamplePass.h"
#include "instro/example/ExampleConstructSet.h"
#include "instro/example/ExampleConstructPrinter.h"

namespace InstRO {
namespace Example {

void ExampleConstructPrinter::execute() {
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface cs(inputPass->getOutput());
	std::cout << "ConstructSet contains " << cs.size() << " constructs:" << std::endl;
	unsigned long count = 0;
	for (auto construct : cs) {
		InstRO::Core::Construct* pc = construct.get();
		InstRO::Example::Core::ExampleConstruct* ec =
				dynamic_cast<InstRO::Example::Core::ExampleConstruct*>(construct.get());
		std::cout << count << ":\t Level " << ec->getTraits().toString() << "\t:" << ec->toString() << std::endl;
		count++;
	}
	outputSet = *(inputPass->getOutput());
}
}
}
