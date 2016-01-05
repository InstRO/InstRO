#ifndef INSTRO_PASS_SELECTOR_BOOLEANCOMPOUNDSELECTOR_H
#define INSTRO_PASS_SELECTOR_BOOLEANCOMPOUNDSELECTOR_H

#include "instro/core/PassImplementation.h"
#include "instro/core/Pass.h"

namespace InstRO {
namespace Selector {

/**
 * \brief Provides boolean operations on the streams of two input selectors
 * \author Roman Ness
 * XXX RN: do we need inverted inputs?
 */
class BooleanCompoundSelector : public InstRO::Core::PassImplementation {

 public:
	enum CompoundOperationType { CO_OR, CO_AND, CO_XOR, CO_MINUS };

	BooleanCompoundSelector(Pass* inputLeft, Pass* inputRight, CompoundOperationType operationType)
			: PassImplementation(InstRO::Core::ChannelConfiguration(inputLeft, inputRight)),
				passLeft(inputLeft),
				passRight(inputRight),
				operationType(operationType) {}

	void execute() override;

 private:
	Pass* passLeft;
	Pass* passRight;

	enum CompoundOperationType operationType;

};

}	// namespace Selector
}	// namepsace InstRO

#endif	// INSTRO_CORE_BOOLEANCOMPOUNDSELECTOR_H
