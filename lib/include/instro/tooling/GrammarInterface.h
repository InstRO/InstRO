#ifndef INSTRO_TOOLING_GRAMMARINTERFACE_H
#define INSTRO_TOOLING_GRAMMARINTERFACE_H

#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Tooling {
namespace GrammarInterface {

class GrammarInterface {
public:
	virtual ~GrammarInterface() {
	}

	virtual std::unique_ptr<InstRO::Core::ConstructSet> getConstructsByClass(
			const InstRO::Core::ConstructTraitType constructClass) = 0;
};

}	// GrammarInterface
}	// Tooling
}	// InstRO
#endif
