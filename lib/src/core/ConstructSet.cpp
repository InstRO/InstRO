#include <vector>
#include "instro/core/ConstructSet.h"

namespace InstRO{
namespace Core{

std::string constructLevelToString(ConstructLevelType type)
{
	switch(type)
	{
	// case 1: return std::string("LiteralConstructLevel");break;

	//case InstRO::Core::ContstructLevelEnum::CLMin:
	case InstRO::Core::ContstructLevelEnum::CLFragment:
		return std::string("CL-Fragment"); break;
	case CLExpression:
		return std::string("CL-Expression"); break;
	case CLStatement:
		return std::string("CL-Statement"); break;
	case CLLoop:
		return std::string("CL-LoopStatement"); break;
	case CLConditional:
		return std::string("CL-ConditionalStatement"); break;
	case CLScope:
		return std::string("CL-ScopeStatement"); break;
	case CLSimple:
		return std::string("CL-SimpleStatement"); break;
	case CLFunction:
		return std::string("CL-FunctionStatement"); break;
	case CLFileScope:
		return std::string("CL-FileScope"); break;
	// case CLMax:		
	case CLGlobalScope:
		return std::string("CL-GlobalScope"); break;
			
	default: return std::string("UnknownConstructLevel");break;
	}
}
std::string operator+(const std::string &lhs,const ConstructLevelType &type)
{
	return std::string(lhs).append(constructLevelToString(type));
	/*switch(type)
	{
	case 1: return std::string(lhs).append(std::string("LiteralConstructLevel"));break;
	default: return std::string(lhs).append(std::string("UnknownConstructLevel"));break;
	}*/
}
}
} // End namespace InstRO
