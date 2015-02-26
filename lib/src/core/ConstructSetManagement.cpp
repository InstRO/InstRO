#include "instro/core/ConstructSetManagement.h"
namespace InstRO
{
namespace Core
{
ConstructSet * PassConstructSetManagement::getInput(PassConstructSetManagement * from)
{
	if (inputOverride.find(from)==inputOverride.end())
	{
		return from->getOutput();
	}
	else
	{
		return inputOverride[from];
	}
	throw std::string("I should no be here. It is impossible!!!");
}

void PassConstructSetManagement::overrideInput(PassConstructSetManagement * from, ConstructSet* overrideSet)
{
	inputOverride[from]=overrideSet;
}
}
}
