#ifndef ROSE_COMPOUNDSELECTORS_H
#define ROSE_COMPOUNDSELECTORS_H
#include <vector>
#include <string>
#include "instro/rose/RosePass.h"

namespace InstRO
{
namespace Rose
{
namespace Selectors
{

class CompoundSelector:public Selector{
public:
	CompoundSelector(RosePass * inputA,RosePass * inputB)
	{		
	};
	void init(){};
	void execute(){};
	void finalize(){};
	void releaseOutput(){};
	ConstructSet*getOuput(){return new ConstructSet();}
};
}// End namespace Selectors
}// End namespace Rose
}// End namepsace InstRO
#endif