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

std::string RoseConstruct::getIdentifier() const {
	std::string identifier("");
	if (isSgLocatedNode(node) != nullptr) {
		auto locatedNode = isSgLocatedNode(node);
		Sg_File_Info *fInfo = locatedNode->get_file_info();
		identifier += fInfo->get_filenameString() + ":" + std::to_string(fInfo->get_line());
		identifier += "--" + constructLevelToString(getTraits().max());
		if (isSgFunctionDefinition(node)) {
			auto fDef = isSgFunctionDefinition(node);
			identifier += "-" + fDef->get_declaration()->get_name().getString();
		}
	} else {
		logIt(WARN) << "The node in Construct " << getID() << " was not a located node." << std::endl;
		raise_exception("The construct was not exportable");
	}
	return identifier;
}

}	// namespace Core
}	// namespace Rose
}	// namespace InstRO

