
#ifndef INSTRO_PASS_SELECTOR_CONSTRUCTCLASSSELECTOR_H_
#define INSTRO_PASS_SELECTOR_CONSTRUCTCLASSSELECTOR_H_

#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Selector {

/**
 * \brief Selects all constructs with a given ConstructClass.
 * \author Roman Ness
 */
class ConstructClassSelector : public InstRO::Core::PassImplementation {
 public:
	ConstructClassSelector(InstRO::Core::ConstructTraitType constructClass)
			: InstRO::Core::PassImplementation(), constructClass(constructClass) {}

	void execute() override;

 private:
	InstRO::Core::ConstructTraitType constructClass;
};

}	// namespace Selector
}	// namespace InstRO

#endif /* LIB_INCLUDE_INSTRO_PASS_SELECTOR_CONSTRUCTCLASSSELECTOR_H_ */
