#ifndef INSTRO_CORE_CONSTRUCTSET_H
#define INSTRO_CORE_CONSTRUCTSET_H 0.1

#include <vector>

namespace InstRO{
	class Pass;
	namespace Core{
		class ConstructSet;
	}
	namespace PassManagement{

class PassEnvelope
{
	friend class PassManager;
	public:
		PassEnvelope(Pass * newPass)
		{
			pass=newPass;
			//			successor=NULL;
		}
	protected:
		Pass * pass;
		//Pass * successor;
		std::vector<Pass*> predecessors;
	};

	} // End namespace PassManagement
} // End namespace InstRO

#endif