#ifndef INSTRO_CORE_CHANNEL_CONFIGURATION_H
#define INSTRO_CORE_CHANNEL_CONFIGURATION_H

#include "instro/core/ConstructTraitType.h"

#include "instro/utility/Logger.h"

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <map>


namespace InstRO {
class Pass;
namespace Core {

/*
 * A channel configuration maps a channel number to an actual Pass.
 * That is, at construction time, the factory wires the respective input channel to the corresponding channel number.
 * Thus, the user retrieves the input for a given channel within a PassImplementation using the getInput(int) method.
 * The class also models the _requirements_ regarding the Construct levels of the Pass's inputs which it imposes.
 */
class ChannelConfiguration {
 public:
	typedef std::pair<int, Pass *> ConfigTuple;

	ChannelConfiguration() {}

	/* Implicitly sets the channel that p corresponds to to zero */
	ChannelConfiguration(Pass *p) {
		if (p == nullptr) {
			throw std::string("Empty pass as argument in Channel Configuration disallowed.");
		}
		inputChannelMap.insert({0, p});
		inputChannelMin[p] = InstRO::Core::ConstructTraitType::CTMin;
		inputChannelMax[p] = InstRO::Core::ConstructTraitType::CTMax;
	}

	/** Must use the ConfigTuple when using this constructor */
	template <class... PassTuplesT>
	ChannelConfiguration(ConfigTuple ct, PassTuplesT... tuples) {
		if (ct.second == nullptr) {
			throw std::string("Empty pass in channel configuration disallowed.");
		}
		inputChannelMap.insert(ct);
		insertionHelper(tuples...);

		for (const auto &t : inputChannelMap) {
			inputChannelMin[t.second] = InstRO::Core::ConstructTraitType::CTMin;
			inputChannelMax[t.second] = InstRO::Core::ConstructTraitType::CTMax;
		}
	}

	/** Explicitly set the level of a given Pass to the min and max Construct levels */
	void setConstructLevel(Pass *inputPass, InstRO::Core::ConstructTraitType minLevel,
												 InstRO::Core::ConstructTraitType maxLevel) {
		inputChannelMin[inputPass] = minLevel;
		inputChannelMax[inputPass] = maxLevel;
	}

	/** Sets the minimal Construct level the Pass expects in the input set for inputPass */
	const InstRO::Core::ConstructTraitType getMinConstructLevel(Pass *inputPass) const {
		return inputChannelMin.at(inputPass);
	}

	/** Sets the maximal Construct level the Pass expects in the input set for inputPass */
	const InstRO::Core::ConstructTraitType getMaxConstructLevel(Pass *inputPass) const {
		return inputChannelMax.at(inputPass);
	}

	/** Generates a list of Passes the pass depends on */
	std::vector<InstRO::Pass *> const getPasses() const {
		std::vector<Pass *> res(inputChannelMap.size());
		std::transform(inputChannelMap.begin(), inputChannelMap.end(), res.begin(),
									 [](std::pair<int, Pass *> p) { return p.second; });
		return res;
	}

	/** Returns the output fetched from the preceeding pass connected to channel */
	InstRO::Core::ConstructSet *getConstructSetForChannel(int channel) const;

 private:
	std::map<int, Pass *> inputChannelMap;
	std::unordered_map<Pass *, InstRO::Core::ConstructTraitType> inputChannelMin;
	std::unordered_map<Pass *, InstRO::Core::ConstructTraitType> inputChannelMax;

	/** Helpers to get the insertion and nullptr checking working */
	template <class PT, class... PassTuplesT>
	void insertionHelper(PT ct, PassTuplesT... tuples) {
		if (ct.second == nullptr) {
			throw std::string("Empty pass as argument in Channel Configuration disallowed.");
		}
		inputChannelMap.insert(ct);
		insertionHelper(tuples...);
	}
	void insertionHelper(ConfigTuple ct) {
		if (ct.second == nullptr) {
			throw std::string("Empty pass as argument in Channel Configuration disallowed.");
		}
		inputChannelMap.insert(ct);
	}
};
}
}

#endif
