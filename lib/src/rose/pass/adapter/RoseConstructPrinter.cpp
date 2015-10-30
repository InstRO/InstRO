#include "instro/core/Pass.h"
#include "instro/core/ConstructSet.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/pass/adapter/RoseConstructPrinter.h"

namespace InstRO {
namespace Rose {
namespace Adapter {

void RoseConstructPrinter::execute() {
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface cs(inputPass->getOutput());
	std::cout << "ConstructSet contains " << cs.size() << " constructs:" << std::endl;

	unsigned long count = 0;
	for (auto construct : cs) {
		InstRO::Rose::Core::RoseConstruct* ec = dynamic_cast<InstRO::Rose::Core::RoseConstruct*>(construct.get());

		std::stringstream ss;
		ss << count << ":\t " << ec->getTraits().toString() << ":\t";

		if (ec->getTraits().is(InstRO::Core::ConstructTraitType::CTFunction)) {
			std::cout << ss.str()
								<< isSgFunctionDefinition(ec->getNode())->get_declaration()->get_qualified_name().getString()
								<< std::endl;
		} else {
			std::cout << ss.str() << ec->getNode()->unparseToString() << std::endl;
		}
		count++;
	}
	outputCS = *(inputPass->getOutput());
}

}	// namespace Adapter
}	// namespace Rose
}	// namespace InstRO
