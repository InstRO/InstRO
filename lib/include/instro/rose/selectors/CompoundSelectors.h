#ifndef ROSE_COMPOUNDSELECTORS_H
#define ROSE_COMPOUNDSELECTORS_H
#include <vector>
#include <string>
#include "instro/rose/RosePass.h"
#include "instro/core/Pass.h" // Pass
#include "instro/core/PassImplementation.h" // ChannelConfiguration

namespace InstRO
{
namespace Rose
{
namespace Selectors
{

class CompoundSelector:public InstRO::Rose::RosePass{
public:
	CompoundSelector(Pass * inputA, Pass * inputB,int Operation):RosePass(InstRO::Core::ChannelConfiguration(inputA,inputB))
	{		
	};
	void init(){};
	void execute(){};
	void finalize(){};
	void releaseOutput(){};
	InstRO::Core::ConstructSet * getOutput() override {
		return new InstRO::Core::ConstructSet(); 
	}
};
}// End namespace Selectors
}// End namespace Rose
}// End namepsace InstRO
#endif