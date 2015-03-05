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