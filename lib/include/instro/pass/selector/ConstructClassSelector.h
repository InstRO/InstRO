
#ifndef LIB_INCLUDE_INSTRO_PASS_SELECTOR_CONSTRUCTCLASSSELECTOR_H_
#define LIB_INCLUDE_INSTRO_PASS_SELECTOR_CONSTRUCTCLASSSELECTOR_H_

#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Selector {

/**
 * \brief Selects all constructs with a given ConstructClass.
 * \author Roman Ness
 */
class ConstructClassSelector : public InstRO::Core::PassImplementation {
 public:
	ConstructClassSelector(InstRO::Core::ConstructTraitType constructClass) :
			InstRO::Core::PassImplementation(InstRO::Core::ChannelConfiguration()),
			constructClass(constructClass) {
	}

	void init() override {}
 	void execute() override;
 	void finalize() override {}
	void releaseOutput() override {
		output->clear();
	}
	InstRO::Core::ConstructSet *getOutput() override {
		return output.get();
	}

 private:
 	std::unique_ptr<InstRO::Core::ConstructSet> output;
 	InstRO::Core::ConstructTraitType constructClass;
};

}	// namespace Selector
}	// namespace InstRO

#endif /* LIB_INCLUDE_INSTRO_PASS_SELECTOR_CONSTRUCTCLASSSELECTOR_H_ */
