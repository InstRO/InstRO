#include <memory>
#include <set>
#include <iostream>
#include "instro/core/ConstructSet.h"
#include "instro/rose/core/RoseConstructSet.h"

#include "rose.h"

namespace InstRO {
namespace Rose {

std::shared_ptr<InstRO::Rose::Core::RoseConstruct> toRoseConstruct(std::shared_ptr<InstRO::Core::Construct> c) {
	return std::dynamic_pointer_cast<InstRO::Rose::Core::RoseConstruct>(c);
}

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
		logIt(DEBUG) << "getIdentifier(): \nNode type: " << locatedNode->class_name() << std::endl;
	
		Sg_File_Info *fInfo = locatedNode->get_file_info();
		identifier += fInfo->get_filenameString() + ":" + std::to_string(fInfo->get_line());
		identifier += "--" + generateConstructIdentifier();
		
		if (isSgFunctionDefinition(node)) {
			auto fDef = isSgFunctionDefinition(node);
			identifier += "-" + fDef->get_declaration()->get_qualified_name().getString();
		}

		logIt(DEBUG) << "Generated identifier: " << identifier << std::endl;
	} else {
		logIt(WARN) << "The node in Construct " << getID() << " was not a located node." << std::endl;
		raise_exception("The construct was not exportable");
	}
	return identifier;
}

std::string RoseConstruct::generateConstructIdentifier() const {
	using CTLvl = InstRO::Core::ConstructTraitType;
	auto cts = getTraits();
	if(cts.is(CTLvl::CTConditionalStatement)){
		return "ConditionalStatement";
	}
	if(cts.is(CTLvl::CTLoopStatement)){
		return "LoopStatement";
	}
	if(cts.is(CTLvl::CTScopeStatement)){
		return "ScopeStatement";
	}
	if(cts.is(CTLvl::CTSimpleStatement)){
		return "SimpleStatement";
	}
	if(cts.is(CTLvl::CTWrappableStatement)){
		return "WrappableStatement";
	}
	
	return constructLevelToString(cts.max());
}

}	// namespace Core
}	// namespace Rose
}	// namespace InstRO

