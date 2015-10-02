#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface
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

	std::unique_ptr<InstRO::Core::ConstructSet> getConstructsByType(
			const InstRO::Tooling::GrammarInterface::GrammarTypesType &types) override;

	std::list<InstRO::Tooling::GrammarInterface::GrammarTypesType> getGrammerTypes(
			const InstRO::Core::ConstructSet &cs) override;

	std::unique_ptr<InstRO::Core::ConstructSet> getConstructsByClass(
			const InstRO::Core::ConstructTraitType constructClass) override;
};
}
}
}
}
