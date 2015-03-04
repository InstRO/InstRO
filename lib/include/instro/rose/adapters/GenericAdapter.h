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
				registerInputPass(functionInstrumentation,Core::ConstructLevelStatement);
				registerInputPass(loopInstrumentation,Core::ConstructLevelStatement);
				registerInputPass(loopBodyInstrumentation,Core::ConstructLevelStatement);
			}
		};

	}
}
}

#endif