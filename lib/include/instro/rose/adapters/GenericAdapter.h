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
				setRequiresInput();
				unsetProvidesOuput();
				setPassName("ROSE_GenericAdapter");
				if (functionInstrumentation!=NULL) registerInputPass(functionInstrumentation,Core::ConstructLevelStatement);
				if (loopInstrumentation!=NULL) registerInputPass(loopInstrumentation,Core::ConstructLevelStatement);
				if (loopBodyInstrumentation!=NULL) registerInputPass(loopBodyInstrumentation,Core::ConstructLevelStatement);
			}
		};

	}
}
}

#endif