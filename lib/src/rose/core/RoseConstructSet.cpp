#include <memory>
#include <set>
#include <iostream>
#include "instro/core/ConstructSet.h"
#include "instro/rose/core/RoseConstructSet.h"

#include "rose.h"

namespace InstRO {
namespace Rose {
namespace Core {

namespace RoseConstructLevelPredicates {

using namespace RoseConstructLevelPredicates;
CTPredicate getPredicateForTraitType(InstRO::Core::ConstructTraitType traitType) {
	switch (traitType) {
		case InstRO::Core::ConstructTraitType::CTExpression:
			return CLExpressionPredicate();
		case InstRO::Core::ConstructTraitType::CTStatement:
			return CLStatementPredicate();
		case InstRO::Core::ConstructTraitType::CTLoopStatement:
			return CLLoopPredicate();
		case InstRO::Core::ConstructTraitType::CTConditionalStatement:
			return CLConditionalPredicate();
		case InstRO::Core::ConstructTraitType::CTScopeStatement:
			return CLScopeStatementPredicate();
		case InstRO::Core::ConstructTraitType::CTSimpleStatement:
			return CLSimpleStatementPredicate();
		case InstRO::Core::ConstructTraitType::CTWrappableStatement:
			return CTWrappableStatementPredicate();
		case InstRO::Core::ConstructTraitType::CTFunction:
			return CLFunctionPredicate();
		case InstRO::Core::ConstructTraitType::CTFileScope:
			return CLFileScopePredicate();
		case InstRO::Core::ConstructTraitType::CTGlobalScope:
			return CLGlobalScopePredicate();
	}
}

}	// namespace RoseConstructLevelPredicates

}	// namespace Core
}	// namespace Rose
}	// namespace InstRO
//
// struct InstrumentableConstructPredicate {
//	bool operator()(SgNode* n) const {
//		if (isSgDoWhileStmt(n) || isSgBasicBlock(n) || isSgFunctionDefinition(n))
//			return true;
//		if (isSgExpression(n) != nullptr)
//			return true;
//		return false;
//	}
//};
