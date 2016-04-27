#include "instro/core/ConstructTraitType.h"
#include "instro/utility/exception.h"


namespace InstRO {
namespace Core {

std::string toString(const ConstructTraitType t) { return constructLevelToString(t); }

ConstructTraitType getConstructTraitTypeFromString(std::string cttString) {
	if (cttString == "min") {
		return InstRO::Core::ConstructTraitType::CTMin;
	} else if (cttString == "expression") {
		return InstRO::Core::ConstructTraitType::CTExpression;
	} else if (cttString == "statement") {
		return InstRO::Core::ConstructTraitType::CTStatement;
	} else if (cttString == "loopStatement") {
		return InstRO::Core::ConstructTraitType::CTLoopStatement;
	} else if (cttString == "conditionalStatement") {
		return InstRO::Core::ConstructTraitType::CTConditionalStatement;
	} else if (cttString == "scopeStatement") {
		return InstRO::Core::ConstructTraitType::CTScopeStatement;
	} else if (cttString == "simpleStatement") {
		return InstRO::Core::ConstructTraitType::CTSimpleStatement;
	} else if (cttString == "wrappableStatement") {
		return InstRO::Core::ConstructTraitType::CTWrappableStatement;
	} else if (cttString == "function") {
		return InstRO::Core::ConstructTraitType::CTFunction;
	} else if (cttString == "fileScope") {
		return InstRO::Core::ConstructTraitType::CTFileScope;
	} else if (cttString == "globalScope") {
		return InstRO::Core::ConstructTraitType::CTGlobalScope;
	} else if (cttString == "max") {
		return InstRO::Core::ConstructTraitType::CTMax;
	} else {
		InstRO::raise_exception("InstRO::Core::getConstructTraitTypeFromString : Unknown ConstructTraitType '" + cttString + "'");
	}
}

std::string constructLevelToString(ConstructTraitType type) {
	switch (type) {
		case ConstructTraitType::CTFragment:
			return std::string("Fragment");
			break;
		case ConstructTraitType::CTExpression:
			return std::string("Expression");
			break;
		case ConstructTraitType::CTStatement:
			return std::string("Statement");
			break;
		case ConstructTraitType::CTWrappableStatement:
			return std::string("WrappableStatement");
			break;
		case ConstructTraitType::CTLoopStatement:
			return std::string("LoopStatement");
			break;
		case ConstructTraitType::CTConditionalStatement:
			return std::string("ConditionalStatement");
			break;
		case ConstructTraitType::CTScopeStatement:
			return std::string("ScopeStatement");
			break;
		case ConstructTraitType::CTSimpleStatement:
			return std::string("SimpleStatement");
			break;
		case ConstructTraitType::CTFunction:
			return std::string("Function");
			break;
		case ConstructTraitType::CTFileScope:
			return std::string("FileScope");
			break;
		case ConstructTraitType::CTGlobalScope:
			return std::string("GlobalScope");
			break;

		default:
			return std::string("Invalid ConstructTrait [") + std::to_string(static_cast<unsigned int>(type)) +
						 std::string("]");
			break;
	}
}
std::string constructLevelToStringShort(ConstructTraitType type) {
	switch (type) {
		case ConstructTraitType::CTFragment:
			return std::string("Frag");
			break;
		case ConstructTraitType::CTExpression:
			return std::string("Expr");
			break;
		case ConstructTraitType::CTStatement:
			return std::string("Stmt");
			break;
		case ConstructTraitType::CTWrappableStatement:
			return std::string("WSmt");
			break;
		case ConstructTraitType::CTLoopStatement:
			return std::string("Loop");
			break;
		case ConstructTraitType::CTConditionalStatement:
			return std::string("Cond");
			break;
		case ConstructTraitType::CTScopeStatement:
			return std::string("ScopeStatement");
			break;
		case ConstructTraitType::CTSimpleStatement:
			return std::string("SSmt");
			break;
		case ConstructTraitType::CTFunction:
			return std::string("Func");
			break;
		case ConstructTraitType::CTFileScope:
			return std::string("File");
			break;
		case ConstructTraitType::CTGlobalScope:
			return std::string("Glob");
			break;

		default:
			return std::string("Invalid ConstructTrait");
			break;
	}
}

std::string operator+(const std::string& lhs, const ConstructTraitType& type) {
	return std::string(lhs).append(constructLevelToString(type));
}

}	// Core
}	// InstRO
