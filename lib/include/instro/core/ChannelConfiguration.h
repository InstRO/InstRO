#ifndef INSTRO_CORE_CHANNEL_CONFIGURATION_H
#define INSTRO_CORE_CHANNEL_CONFIGURATION_H

#include <vector>
#include <unordered_map>
#include <cassert>
#include <map>

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
	typedef std::pair<int, Pass *> ConfigTuple;

	ChannelConfiguration() {}

	/* Implicitly sets the channel that p corresponds to to zero */
	ChannelConfiguration(Pass *p) {
		inputChannelMap.insert({0, p});
		inputChannelMin[p] = InstRO::Core::ConstructTraitType::CTMin;
		inputChannelMax[p] = InstRO::Core::ConstructTraitType::CTMax;
	}

	template <class... PassTuplesT>
	ChannelConfiguration(ConfigTuple ct, PassTuplesT... tuples) {
		inputChannelMap.insert({ct, tuples...});

		for (const auto &t : inputChannelMap) {
			inputChannelMin[t.second] = InstRO::Core::ConstructTraitType::CTMin;
			inputChannelMax[t.second] = InstRO::Core::ConstructTraitType::CTMax;
		}
	}

	/** The user needs to make sure, that the iterator iterates in a sensible way. That is, the elements will be
	 * enumerated ascendingly in the order the iterator iterates over its elements. */
#if 0
	template <class Iterator>
	ChannelConfiguration(Iterator begin, Iterator end, InstRO::Core::ConstructTraitType minLevel,
											 InstRO::Core::ConstructTraitType maxLevel) {
		int count = 0;
		for (Iterator iter = begin; iter != end; ++iter) {
			inputChannelPasses.push_back(*iter);
			inputChannelMap.insert({count, *iter});
			inputChannelMin[*iter] = minLevel;
			inputChannelMax[*iter] = maxLevel;
			++count;
		}
	}
#endif

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

	[[deprecated]] std::vector<InstRO::Pass *> const getPasses() const { return inputChannelPasses; }

	InstRO::Core::ConstructSet *operator[](int pos) const;

 private:
	std::vector<Pass *> inputChannelPasses;
	std::map<int, Pass *> inputChannelMap;
	std::unordered_map<Pass *, InstRO::Core::ConstructTraitType> inputChannelMin;
	std::unordered_map<Pass *, InstRO::Core::ConstructTraitType> inputChannelMax;
};
}
}

#endif
