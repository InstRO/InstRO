#ifndef INSTRO_CORE_PASSIMPLEMENTATION_H
#define INSTRO_CORE_PASSIMPLEMENTATION_H

#include <unordered_map>

#include "instro/core/ConstructSet.h"

namespace InstRO {
class Pass;
namespace Core {
namespace Support {
/*
 * Support class to hand the inputs from the pass manager to the
 * PassImplementation
 */
class InputAggregation {
 public:
	InputAggregation(){};
	InputAggregation(std::unordered_map<InstRO::Pass *, ConstructSet *> m) : pToCMap(m) {}
	ConstructSet *getConstructSet(InstRO::Pass *p) { return pToCMap[p]; }

 private:
	// CI: Us the Pass as a AccessHandle to the RespectiveConstruct set
	std::unordered_map<InstRO::Pass *, InstRO::Core::ConstructSet *> pToCMap;
};
}
}

/**
 * The ChannelConfiguration class abstracts away how a PassImplementation actually relies on its inputs. In this object
 * is encoded which predecessors are existent. Or so.
 */
class ChannelConfiguration {
 public:
	typedef std::tuple<Pass *, int, int> PassInfoTuple;

 protected:
	std::vector<PassInfoTuple> inputChannelPasses;

 public:
	/* Creates a default empty configuration */
	ChannelConfiguration() = default;
	/* Adds the one specified PassInfoTuple */
	ChannelConfiguration(PassInfoTuple passInfo) : inputChannelPasses(1, passInfo) {}
	/* Adds one or more PassInfoPack to the Configuration */
	template <class... PassInfoPack>
	ChannelConfiguration(PassInfoTuple passInfo, PassInfoPack... passInfos)
			: inputChannelPasses({passInfo, passInfos...}) {}

	// XXX JP: Why would we want that?
	void setConstructLevel(Pass *inputPass, int minLevel, int maxLevel) {}
};

/*
 * This class is the user-interface to create his own pass.
 * One needs to inherit from this class and implement a compiler dependent pass.
 * Using the getInput(passId) one can retrieve the ConstructSet of one of the
 * predecessors.
 */
class PassImplementation {
 private:
	ChannelConfiguration cfg;

 protected:
	ChannelConfiguration &getChannelCFG() { return cfg; }

 public:
	ChannelConfiguration const channelCFG() { return cfg; }

	PassImplementation() = delete;

	PassImplementation(ChannelConfiguration cfg) : cfg(cfg) {}

	virtual void init() = 0;
	virtual void execute() = 0;
	virtual void finalize() = 0;
	virtual void releaseOutput() = 0;
	virtual Core::ConstructSet *getOutput() = 0;

	void setInputAggregation(InstRO::Core::Support::InputAggregation ia) { this->ia = ia; }

 protected:
	InstRO::Core::ConstructSet *getInput(InstRO::Pass *pId) { return ia.getConstructSet(pId); }

 private:
	InstRO::Core::Support::InputAggregation ia;
};
}

#endif
