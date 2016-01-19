#ifndef INSTRO_CORE_CHANNEL_CONFIGURATION_H
#define INSTRO_CORE_CHANNEL_CONFIGURATION_H

#include <vector>
#include <unordered_map>
#include <cassert>

#include "instro/core/ConstructSet.h"

#include "instro/utility/Logger.h"

namespace InstRO {
class Pass;
namespace Core {

/*
 * The class serves as interface between Pass and PassImplementation to make them independent of the actual input
 * channel
 */
class ChannelConfiguration {
 public:
	typedef std::pair<int, Pass *> configTuple;

	ChannelConfiguration() {}

	ChannelConfiguration(Pass *p1) {
		inputChannelPasses.push_back(p1);
		inputChannelMin[p1] = InstRO::Core::ConstructTraitType::CTMin;
		inputChannelMax[p1] = InstRO::Core::ConstructTraitType::CTMax;
	}

	template <class... PassList>
	ChannelConfiguration(Pass *p1, PassList... passes) {
		inputChannelPasses.insert(inputChannelPasses.begin(), {p1, passes...});

		std::vector<Pass *> allPasses = {p1, passes...};
		for (auto pass : allPasses) {
			inputChannelMin[pass] = InstRO::Core::ConstructTraitType::CTMin;
			inputChannelMax[pass] = InstRO::Core::ConstructTraitType::CTMax;
		}
	}

	template <class Iterator>
	ChannelConfiguration(Iterator begin, Iterator end, InstRO::Core::ConstructTraitType minLevel,
											 InstRO::Core::ConstructTraitType maxLevel) {
		for (Iterator iter = begin; iter != end; ++iter) {
			inputChannelPasses.push_back(*iter);
			inputChannelMin[*iter] = minLevel;
			inputChannelMax[*iter] = maxLevel;
		}
	}

	void setConstructLevel(Pass *inputPass, InstRO::Core::ConstructTraitType minLevel,
												 InstRO::Core::ConstructTraitType maxLevel) {
		inputChannelMin[inputPass] = minLevel;
		inputChannelMax[inputPass] = maxLevel;
	}

	const InstRO::Core::ConstructTraitType getMinConstructLevel(Pass *inputPass) const {
		return inputChannelMin.at(inputPass);
	}
	const InstRO::Core::ConstructTraitType getMaxConstructLevel(Pass *inputPass) const {
		return inputChannelMax.at(inputPass);
	}
	std::vector<InstRO::Pass *> const getPasses() const { return inputChannelPasses; }

	InstRO::Core::ConstructSet *operator[](int pos) const;

 private:
	std::vector<Pass *> inputChannelPasses;
	std::unordered_map<Pass *, InstRO::Core::ConstructTraitType> inputChannelMin;
	std::unordered_map<Pass *, InstRO::Core::ConstructTraitType> inputChannelMax;
};
}
}

#endif
