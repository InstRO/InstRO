#ifndef INSTRO_ROSE_CYGPROFILEADAPTER_H
#define INSTRO_ROSE_CYGPROFILEADAPTER_H
#include "instro/core/Pass.h" // Pass
#include "instro/core/PassImplementation.h" // ChannelConfiguration
#include "instro/core/ConstructSet.h"
#include "instro/rose/RosePass.h"
namespace InstRO
{
namespace Rose
{
	namespace Adapters
	{
		class CygProfileAdapter: public InstRO::Rose::RosePass
		{
		public:
			std::string className(){return std::string("CygProfileAdapter");}
			CygProfileAdapter(Pass * pass) :RosePass(InstRO::Core::ChannelConfiguration(pass))
			{
			}
				void init(){};
	void execute(){};
	void finalize(){};
	void releaseOutput(){};
	InstRO::Core::ConstructSet *getOutput() override{
		return new InstRO::Core::ConstructSet();
	}
		};

	}
}
}

#endif