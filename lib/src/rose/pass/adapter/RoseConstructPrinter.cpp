#include "instro/core/Pass.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/pass/adapter/RoseConstructPrinter.h"

namespace InstRO {
namespace Rose {
namespace Adapter {

void RoseConstructPrinter::execute(){
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface cs(inputPass->getOutput());
	/*InstRO::Example::Core::InfracstructureInterface::ExampleConstructSet * pecs =
	dynamic_cast<InstRO::Example::Core::InfracstructureInterface::ExampleConstructSet *>(ocs);
	InstRO::Example::Core::InfracstructureInterface::ExampleConstructSet cs = *(pecs);*/
	std::cout << "ConstructSet contains " << cs.size() << " constructs:" << std::endl;
	unsigned long count = 0;
	for (auto construct : cs) {
		InstRO::Core::Construct* pc = construct.get();
		InstRO::Rose::Core::RoseConstruct* ec =
			dynamic_cast<InstRO::Rose::Core::RoseConstruct *>(construct.get());
		std::cout << count << ":\t Level " << ec->getLevel() << "\t:" << ec->getNode()->unparseToString() << std::endl;
		count++;
	}
}

}
}
}
