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
	CTOpenMPStatement = 5,
	CTLoopStatement = 6,
	CTConditionalStatement = 7,
	CTScopeStatement = 8,
	CTSimpleStatement = 9,
	// a statement with observable behavior. No "pure" declarations, namespaces, classes, etc.
	// Wrappable statements
	CTWrappableStatement = 10,
	CTFunction = 11,
	CTFileScope = 12,
	CTGlobalScope = 13,
	CTMax = 14
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
 * I'd suggest those are more for internal work
 */
std::string constructLevelToString(ConstructTraitType type);
std::string constructLevelToStringShort(ConstructTraitType type);
std::string operator+(const std::string& lhs, const ConstructTraitType& type);
}	// Core
}	// InstRO

#endif
