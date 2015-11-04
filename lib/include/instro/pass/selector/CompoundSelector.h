#ifndef INSTRO_CORE_BOOLEANCOMPOUNDSELECTOR_H
#define INSTRO_CORE_BOOLEANCOMPOUNDSELECTOR_H
#include <vector>
#include <string>
#include <memory>
#include "instro/rose/core/RosePassImplementation.h"
#include "instro/core/Pass.h"								 // Pass
#include "instro/core/PassImplementation.h"	// ChannelConfiguration

namespace InstRO {
namespace Selector {

class CompoundSelector : public InstRO::Core::PassImplementation {

 public:
	typedef enum { CO_Or, CO_And, CO_XOr } CompoundOperationType;

	CompoundSelector(Pass* inputA, Pass* inputB, CompoundOperationType Operation)
			: PassImplementation(InstRO::Core::ChannelConfiguration(inputA, inputB)){};
	CompoundSelector(Pass* inputA, bool invertPassA, Pass* inputB, bool invertPassB, CompoundOperationType Operation)
			: PassImplementation(InstRO::Core::ChannelConfiguration(inputA, inputB)){};
	void init() override{};
	void execute() override;
	void finalize() override{};
};
}	// namespace Selector
}	// namepsace InstRO

#endif	// INSTRO_CORE_BOOLEANCOMPOUNDSELECTOR_H
