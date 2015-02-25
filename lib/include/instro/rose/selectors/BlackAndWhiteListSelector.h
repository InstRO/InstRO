#ifndef ROSE_BLACKANDWHITELISTSELECTOR_H
#define ROSE_BLACKANDWHITELISTSELECTOR_H
#include <vector>
#include <string>
#include "instro/rose/RosePass.h"

namespace InstRO
{
namespace Rose
{
namespace Selectors
{

class BlackAndWhiteListSelector:public Selector{
	BlackAndWhiteListSelector(std::vector<std::string> matchRules){};
};
}// End namespace Selectors
}// End namespace Rose
}// End namepsace InstRO
#endif