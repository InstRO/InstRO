#ifndef ROSE_COMPOUNDSELECTORS_H
#define ROSE_COMPOUNDSELECTORS_H
#include <vector>
#include <string>
#include <memory>
#include "instro/rose/core/RosePassImplementation.h"
#include "instro/core/Pass.h"								 // Pass
#include "instro/core/PassImplementation.h"	// ChannelConfiguration

namespace InstRO {
namespace Rose {
namespace Selector {

class CompoundSelector : public InstRO::Rose::RosePassImplementation {

 public:
	typedef enum { CO_Or, CO_And, CO_XOr } CompoundOperationType;
	CompoundSelector(Pass* inputA, Pass* inputB, CompoundOperationType Operation)
			: RosePassImplementation(InstRO::Core::ChannelConfiguration(inputA, inputB)){};
	CompoundSelector(Pass* inputA, bool invertPassA, Pass* inputB, bool invertPassB, CompoundOperationType Operation)
			: RosePassImplementation(InstRO::Core::ChannelConfiguration(inputA, inputB)){};
	void init() override{};
	void execute() override;
	void finalize() override{};
};
}	// End namespace Selectors
}	// End namespace Rose
}	// End namepsace InstRO
#endif
