#include "instro/core/ConstructTraitType.h"
#include "instro/utility/exception.h"


namespace InstRO {
namespace Core {

std::string toString(const ConstructTraitType t) { return constructLevelToString(t); }

ConstructTraitType getConstructTraitTypeFromString(std::string cttString) {
	if (cttString == "Min") {
		return InstRO::Core::ConstructTraitType::CTMin;
	} else if (cttString == "Expression") {
		return InstRO::Core::ConstructTraitType::CTExpression;
	} else if (cttString == "Statement") {
		return InstRO::Core::ConstructTraitType::CTStatement;
	} else if(cttString == "OpenMPStatement"){
		return InstRO::Core::ConstructTraitType::CTOpenMPStatement;
	} else if (cttString == "LoopStatement") {
		return InstRO::Core::ConstructTraitType::CTLoopStatement;
	} else if (cttString == "ConditionalStatement") {
		return InstRO::Core::ConstructTraitType::CTConditionalStatement;
	} else if (cttString == "ScopeStatement") {
		return InstRO::Core::ConstructTraitType::CTScopeStatement;
	} else if (cttString == "SimpleStatement") {
		return InstRO::Core::ConstructTraitType::CTSimpleStatement;
	} else if (cttString == "WrappableStatement") {
		return InstRO::Core::ConstructTraitType::CTWrappableStatement;
	} else if (cttString == "Function") {
		return InstRO::Core::ConstructTraitType::CTFunction;
	} else if (cttString == "FileScope") {
		return InstRO::Core::ConstructTraitType::CTFileScope;
	} else if (cttString == "GlobalScope") {
		return InstRO::Core::ConstructTraitType::CTGlobalScope;
	} else if (cttString == "Max") {
		return InstRO::Core::ConstructTraitType::CTMax;
	} else {
		InstRO::raise_exception("InstRO::Core::getConstructTraitTypeFromString : Unknown ConstructTraitType '" + cttString + "'");
	}
}

std::string constructLevelToString(ConstructTraitType type) {
	switch (type) {
		case ConstructTraitType::CTFragment:
			return std::string("Fragment");
		case ConstructTraitType::CTExpression:
			return std::string("Expression");
		case ConstructTraitType::CTStatement:
			return std::string("Statement");
		case ConstructTraitType::CTWrappableStatement:
			return std::string("WrappableStatement");
		case ConstructTraitType::CTOpenMPStatement:
			return std::string("OpenMPStatement");
		case ConstructTraitType::CTLoopStatement:
			return std::string("LoopStatement");
		case ConstructTraitType::CTConditionalStatement:
			return std::string("ConditionalStatement");
		case ConstructTraitType::CTScopeStatement:
			return std::string("ScopeStatement");
		case ConstructTraitType::CTSimpleStatement:
			return std::string("SimpleStatement");
		case ConstructTraitType::CTFunction:
			return std::string("Function");
		case ConstructTraitType::CTFileScope:
			return std::string("FileScope");
		case ConstructTraitType::CTGlobalScope:
			return std::string("GlobalScope");

		default:
			return std::string("Invalid ConstructTrait [") + std::to_string(static_cast<unsigned int>(type)) +
						 std::string("]");
	}
}
std::string constructLevelToStringShort(ConstructTraitType type) {
	switch (type) {
		case ConstructTraitType::CTFragment:
			return std::string("Frag");
		case ConstructTraitType::CTExpression:
			return std::string("Expr");
		case ConstructTraitType::CTStatement:
			return std::string("Stmt");
		case ConstructTraitType::CTWrappableStatement:
			return std::string("WSmt");
		case ConstructTraitType::CTOpenMPStatement:
			return std::string("OMP");
		case ConstructTraitType::CTLoopStatement:
			return std::string("Loop");
		case ConstructTraitType::CTConditionalStatement:
			return std::string("Cond");
		case ConstructTraitType::CTScopeStatement:
			return std::string("ScopeStatement");
		case ConstructTraitType::CTSimpleStatement:
			return std::string("SSmt");
		case ConstructTraitType::CTFunction:
			return std::string("Func");
		case ConstructTraitType::CTFileScope:
			return std::string("File");
		case ConstructTraitType::CTGlobalScope:
			return std::string("Glob");

		default:
			return std::string("Invalid ConstructTrait");
	}
}

std::string operator+(const std::string& lhs, const ConstructTraitType& type) {
	return std::string(lhs).append(constructLevelToString(type));
}

}	// Core
}	// InstRO
