#ifndef INSTRO_CORE_PASSMANAGER_H
#define INSTRO_CORE_PASSMANAGER_H

#include <vector>

// CI: This Class Extends the PassManger to enable output of the configuration graph
#include "instro/core/PassManager.h"

namespace InstRO{

class Pass;

namespace Core{
class ConstructSet;
} //End namespace Core

namespace PassManagement{
class VisualizingPassManager: public PassManager
{
	public:
		void outputSelectionConfiguration(std::string fileName);
};


} // End namespace PassManagement
} // End namespace InstRO

#endif