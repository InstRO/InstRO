#ifndef INSTRO_TOOLING_CONSTRUCT_CLASS_INTERFACE_H
#define INSTRO_TOOLING_CONSTRUCT_CLASS_INTERFACE_H

#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Tooling {
namespace ConstructClassInterface {

class ConstructClassInterface {
public:
	virtual ~ConstructClassInterface() {
	}

	virtual InstRO::Core::ConstructSet getConstructsByClass(const InstRO::Core::ConstructTraitType constructClass) = 0;
};

}	// GrammarInterface
}	// Tooling
}	// InstRO
#endif
