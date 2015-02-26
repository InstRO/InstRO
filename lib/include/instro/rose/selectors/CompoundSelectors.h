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
	CompoundSelector(Selector * inputA,Selector * inputB)
	{
		setRequiresInput();
		setProvidesOuput();
		setOutputLevel(Core::ConstructLevelMin);
		registerInputPass(inputA,Core::ConstructLevelMin);
		registerInputPass(inputB,Core::ConstructLevelMin);
		
	};
	std::string passName(){return std::string("ROSE-CompoundSelector");}
};
}// End namespace Selectors
}// End namespace Rose
}// End namepsace InstRO
#endif