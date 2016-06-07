#ifndef INSTRO_PASS_SELECTOR_CONSTRUCTCLASSSELECTOR_H_
#define INSTRO_PASS_SELECTOR_CONSTRUCTCLASSSELECTOR_H_

#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Selector {

/**
 * \brief Selects all constructs with a given ConstructTrait.
 * \author Roman Ness
 */
class ConstructTraitSelector : public InstRO::Core::PassImplementation {
 public:
	ConstructTraitSelector(InstRO::Core::ConstructTraitType constructTrait)
			: InstRO::Core::PassImplementation(), constructTrait(constructTrait) {}

	void execute() override;

 private:
	InstRO::Core::ConstructTraitType constructTrait;
};

}	// namespace Selector
}	// namespace InstRO

#endif /* LIB_INCLUDE_INSTRO_PASS_SELECTOR_CONSTRUCTCLASSSELECTOR_H_ */
