#ifndef INSTRO_ROSE_GENERICADAPTER_H
#define INSTRO_ROSE_GENERICADAPTER_H
#include "instro/core/Pass.h"								 // Pass
#include "instro/core/PassImplementation.h"	// ChannelConfiguration
#include "instro/core/ConstructSet.h"

#include "instro/rose/RosePass.h"

namespace InstRO {
namespace Rose {
namespace Adapters {
class GenericAdapter : public InstRO::Rose::RosePass {
 public:
	std::string className() { return std::string("GenericAdapter"); }
	GenericAdapter(Pass *functionInstrumentation, Pass *loopInstrumentation, Pass *loopBodyInstrumentation)
			: RosePass(InstRO::Core::ChannelConfiguration(functionInstrumentation, loopInstrumentation,
																										loopBodyInstrumentation)) {}
	void init(){};
	void execute(){};
	void finalize(){};
	void releaseOutput(){};
	InstRO::Core::ConstructSet *getOutput() override { return new InstRO::Core::ConstructSet(); }
};
}
}
}

#endif