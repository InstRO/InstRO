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

		Sg_File_Info *fInfo = locatedNode->get_startOfConstruct();
		std::string filename(determineCorrectFilename());
		std::string lineInfo(std::to_string(determineCorrectLineInfo()));
		std::string constructString(specificConstructClassToString());
		std::string columnInfo(std::to_string(determineCorrectColumnInformation()));

		identifier += filename + ":" + lineInfo + ":";
		identifier += columnInfo + "--" + constructString;

		if (isSgFunctionDefinition(node)){
			auto fDecl = isSgFunctionDefinition(node)->get_declaration();
			identifier += "-" + fDecl->get_qualified_name().getString();
		}

		logIt(DEBUG) << "Generated identifier: " << identifier << std::endl;
	} else {
		logIt(ERROR) << "The node in Construct " << getID() << " was not a located node." << std::endl;
		throw std::string("The construct was not exportable");
	}
	return identifier;
}

int RoseConstruct::determineCorrectLineInfo() const {

	// Empty increment expression in for loop gets correct line info
	if(isSgNullExpression(node) && isSgForStatement(node->get_parent())){
		return isSgLocatedNode(node->get_parent())->get_startOfConstruct()->get_line();
	}

	return isSgLocatedNode(node)->get_startOfConstruct()->get_line();
}

std::string RoseConstruct::determineCorrectFilename() const {

	// empty increment expression in for loop gets correct file name
	if(isSgNullExpression(node) && isSgForStatement(node->get_parent())){
		return isSgLocatedNode(node->get_parent())->get_startOfConstruct()->get_filenameString();
	}

	return isSgLocatedNode(node)->get_startOfConstruct()->get_filenameString();
}

/*
 * As it seems this is overly complex, we do not retrieve the correct values in all cases.
 * Especially for something like: for(; ; __i){..} we use the start column of the for loop as the
 * Construct column for the empty SimpleStatement constructs for init and test.
 * If something happens we do not account for, the returned column info is -1.
 */
int RoseConstruct::determineCorrectColumnInformation() const {
	int colInfo = -1;
	SgNode *n = node;
	if (isSgNullStatement(n)) {
		// we need to account for the coulmn info in situations like for(;;)STMT;
		if (isSgForInitStatement(n->get_parent())) {
			auto forStmt = isSgForStatement(n->get_parent()->get_parent());
			int soc = forStmt->get_startOfConstruct()->get_col();
			logIt(DEBUG) << "CASE 1: Init Statement is SgNullStatement. Setting Concstruct column to start of for loop"
									 << std::endl;
			return soc;	// This is not accurate
		}

		if (isSgForStatement(n->get_parent())) {
			auto forStmt = isSgForStatement(n->get_parent());
			int soc = forStmt->get_startOfConstruct()->get_col();
			logIt(DEBUG) << "CASE 2: Test Statement is SgNullStatement. Setting Concstruct column to start of for loop"
									 << std::endl;
			return soc;	// This is not accurate
		}
	}

	if (isSgNullExpression(n)){
		if (isSgForStatement(n->get_parent())) {
			auto forStmt = isSgForStatement(n->get_parent());
			int soc = forStmt->get_startOfConstruct()->get_col();
			logIt(DEBUG) << "CASE 3: Increment Statement is SgNullExpression. Setting Concstruct column to start of for loop"
									 << std::endl;
			return soc;	// This is not accurate
		}
		if(isSgExprStatement(n->get_parent()) && isSgForStatement(n->get_parent()->get_parent())){
			int soc = isSgLocatedNode(n->get_parent())->get_startOfConstruct()->get_col();
			return soc;
		}
	}

	if (isSgLocatedNode(n)) {
		colInfo = isSgLocatedNode(n)->get_startOfConstruct()->get_col();
	}

	if (isSgFunctionDefinition(n)) {
		colInfo = isSgFunctionDeclaration(isSgFunctionDefinition(n)->get_parent())->get_startOfConstruct()->get_col();
	}

	return colInfo;
}

std::string RoseConstruct::specificConstructClassToString() const {
	using CTLvl = InstRO::Core::ConstructTraitType;
	auto cts = getTraits();
	if (cts.is(CTLvl::CTConditionalStatement)) {
		return "ConditionalStatement";
	}
	if (cts.is(CTLvl::CTLoopStatement)) {
		return "LoopStatement";
	}
	if (cts.is(CTLvl::CTScopeStatement)) {
		return "ScopeStatement";
	}
	if (cts.is(CTLvl::CTSimpleStatement)) {
		return "SimpleStatement";
	}
	if (cts.is(CTLvl::CTWrappableStatement)) {
		return "WrappableStatement";
	}

	return constructLevelToString(cts.max());
}

}	// namespace Core
}	// namespace Rose
}	// namespace InstRO
