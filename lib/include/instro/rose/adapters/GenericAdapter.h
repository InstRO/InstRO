#ifndef INSTRO_ROSE_GENERICADAPTER_H
#define INSTRO_ROSE_GENERICADAPTER_H

#include "instro/rose/RosePass.h"
namespace InstRO
{
namespace Rose
{
	namespace Adapters
	{
		class GenericAdapter: public InstRO::Rose::Adapters::Adapter
		{
		public:
			std::string className(){return std::string("GenericAdapter");}
			GenericAdapter(RosePass * functionInstrumentation,RosePass * loopInstrumentation,RosePass * loopBodyInstrumentation)
			{
				
			}
				void init(){};
	void execute(){};
	void finalize(){};
	void releaseOutput(){};
	ConstructSet*getOuput(){return new ConstructSet();}
		};

	}
}
}

#endif