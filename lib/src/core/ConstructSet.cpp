#include <vector>
#include "instro/core/ConstructSet.h"

namespace InstRO{
namespace Core{

std::string contstructLevelToString(ContstructLevelType type)
{
	switch(type)
	{
	case 1: return std::string("LiteralConstructLevel");break;
	default: return std::string("UnknownConstructLevel");break;
	}
}
std::string operator+(const std::string &lhs,const ContstructLevelType &type)
{
	switch(type)
	{
	case 1: return std::string(lhs).append(std::string("LiteralConstructLevel"));break;
	default: return std::string(lhs).append(std::string("UnknownConstructLevel"));break;
	}
}
}
} // End namespace InstRO