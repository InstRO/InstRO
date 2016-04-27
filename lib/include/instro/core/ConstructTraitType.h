#ifndef INSTRO_CORE_CONSTRUCT_TRAIT_TYPE_H
#define INSTRO_CORE_CONSTRUCT_TRAIT_TYPE_H

#include <string>

namespace InstRO {
namespace Core {

enum class ConstructTraitType {
	CTNoTraits = 0,	// XXX how to avoid this?
	CTMin = 1,
	// Please do not use fragments. They may become deprecated
	CTFragment = 2,
	// Any expression with observable behavior
	CTExpression = 3,
	CTStatement = 4,
	// separate Loop, Conditional, Scope and Simple Statements
	CTLoopStatement = 5,
	CTConditionalStatement = 6,
	CTScopeStatement = 7,
	CTSimpleStatement = 8,
	// a statement with observable behavior. No "pure" declarations, namespaces, classes, etc.
	// Wrappable statements
	CTWrappableStatement = 9,
	CTFunction = 10,
	CTFileScope = 11,
	CTGlobalScope = 12,
	CTMax = 13
};

/**
 * Generates the Construct Traits name as a string
 */
std::string toString(const ConstructTraitType t);

/**
 * Parses the ConcstructTraits string representation and returns the respective enum class value
 */
ConstructTraitType getConstructTraitTypeFromString(std::string cttString);



/**
 * I'd suggest those are more for internal workk
 */
std::string constructLevelToString(ConstructTraitType type);
std::string constructLevelToStringShort(ConstructTraitType type);
std::string operator+(const std::string& lhs, const ConstructTraitType& type);
}	// Core
}	// InstRO

#endif
