#ifndef INSTRO_ROSE_TOOLING_ROSE_GRAMMAR_INTERFACE_H
#define INSTRO_ROSE_TOOLING_ROSE_GRAMMAR_INTERFACE_H

#include "instro/core/ConstructSet.h"
#include "instro/tooling/GrammarInterface.h"

#include "rose.h"

namespace InstRO {
namespace Rose {
namespace Tooling {

namespace GrammarInterface {

class RoseGrammarInterface : public InstRO::Tooling::GrammarInterface::GrammarInterface {
 protected:
	SgProject *proj;

 public:
	RoseGrammarInterface() = delete;
	RoseGrammarInterface(SgProject *projArg) : proj(projArg) {}

	InstRO::Core::ConstructSet getConstructsByClass(const InstRO::Core::ConstructTraitType constructClass) override;
};
}
}
}
}

#endif
