#ifndef INSTRO_TOOLING_CONSTRUCT_CLASS_INTERFACE_H
#define INSTRO_TOOLING_CONSTRUCT_CLASS_INTERFACE_H

#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Tooling {
namespace ConstructTraitInterface {

class ConstructTraitInterface {
public:
	virtual ~ConstructTraitInterface() {
	}

	virtual InstRO::Core::ConstructSet getConstructsByTrait(const InstRO::Core::ConstructTraitType constructTrait) = 0;
};

}	// GrammarInterface
}	// Tooling
}	// InstRO
#endif
