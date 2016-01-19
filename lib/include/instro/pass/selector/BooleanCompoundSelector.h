#ifndef INSTRO_PASS_SELECTOR_BOOLEANCOMPOUNDSELECTOR_H
#define INSTRO_PASS_SELECTOR_BOOLEANCOMPOUNDSELECTOR_H

#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Selector {

/**
 * \brief Provides boolean operations on the streams of two input selectors
 * \author Roman Ness, Jan-Patrick Lehr
 * XXX RN: do we need inverted inputs?
 */
class BooleanCompoundSelector : public InstRO::Core::PassImplementation {
 public:
	enum CompoundOperationType { CO_OR, CO_AND, CO_XOR, CO_MINUS };

	BooleanCompoundSelector(CompoundOperationType operationType) : operationType(operationType) {}

	void execute() override;

 private:
	enum CompoundOperationType operationType;
};

}	// namespace Selector
}	// namepsace InstRO

#endif	// INSTRO_CORE_BOOLEANCOMPOUNDSELECTOR_H
