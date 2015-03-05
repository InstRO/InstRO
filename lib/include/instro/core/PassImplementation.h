#ifndef INSTRO_CORE_PASSIMPLEMENTATION_H
#define INSTRO_CORE_PASSIMPLEMENTATION_H
#include "instro/core/ConstructSet.h"

//#include "instro/roseInstRO.h"

namespace InstRO{
	// Do the compiler stuff
class PassImplementation
{
public:
	virtual void init()=0;
	virtual void execute()=0;
	virtual void finalize()=0;
	virtual void releaseOutput()=0;
	virtual Core::ConstructSet * getOutput()=0;
};
}

#endif 
