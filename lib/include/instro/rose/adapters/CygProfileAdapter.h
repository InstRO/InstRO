#ifndef INSTRO_ROSE_CYGPROFILEADAPTER_H
#define INSTRO_ROSE_CYGPROFILEADAPTER_H

#include "instro/rose/RosePass.h"
namespace InstRO
{
namespace Rose
{
	namespace Adapters
	{
		class CygProfileAdapter: public InstRO::Rose::Adapters::Adapter
		{
		public:
			std::string className(){return std::string("CygProfileAdapter");}
			CygProfileAdapter(RosePass * pass)
			{
				setRequiresInput();
				unsetProvidesOuput();
				setPassName("ROSE_CygProfileAdapter");
				registerInputPass(pass,Core::ConstructLevelStatement);
			}
		};

	}
}
}

#endif