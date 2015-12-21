#include "instro/rose/core/RoseConstructSet.h"

#include "instro/core/Helper.h"
#include "rose.h"

namespace InstRO {
namespace Rose {

std::shared_ptr<InstRO::Rose::Core::RoseConstruct> toRoseConstruct(std::shared_ptr<InstRO::Core::Construct> c) {
	auto roseConstruct = std::dynamic_pointer_cast<InstRO::Rose::Core::RoseConstruct>(c);
	if (roseConstruct == nullptr) {
		throw std::string("Error: Could not cast to RoseConstruct");
	}
	return roseConstruct;
}

namespace Core {

namespace RoseConstructLevelPredicates {

using namespace RoseConstructLevelPredicates;
std::unique_ptr<CTPredicate> getPredicateForTraitType(InstRO::Core::ConstructTraitType traitType) {
	switch (traitType) {
		case InstRO::Core::ConstructTraitType::CTExpression:
			return std::make_unique<CLExpressionPredicate>(CLExpressionPredicate());
		case InstRO::Core::ConstructTraitType::CTStatement:
			return std::make_unique<CLStatementPredicate>(CLStatementPredicate());
		case InstRO::Core::ConstructTraitType::CTLoopStatement:
			return std::make_unique<CLLoopPredicate>(CLLoopPredicate());
		case InstRO::Core::ConstructTraitType::CTConditionalStatement:
			return std::make_unique<CLConditionalPredicate>(CLConditionalPredicate());
		case InstRO::Core::ConstructTraitType::CTScopeStatement:
			return std::make_unique<CLScopeStatementPredicate>(CLScopeStatementPredicate());
		case InstRO::Core::ConstructTraitType::CTSimpleStatement:
			return std::make_unique<CLSimpleStatementPredicate>(CLSimpleStatementPredicate());
		case InstRO::Core::ConstructTraitType::CTWrappableStatement:
			return std::make_unique<CTWrappableStatementPredicate>(CTWrappableStatementPredicate());
		case InstRO::Core::ConstructTraitType::CTFunction:
			return std::make_unique<CLFunctionPredicate>(CLFunctionPredicate());
		case InstRO::Core::ConstructTraitType::CTFileScope:
			return std::make_unique<CLFileScopePredicate>(CLFileScopePredicate());
		case InstRO::Core::ConstructTraitType::CTGlobalScope:
			return std::make_unique<CLGlobalScopePredicate>(CLGlobalScopePredicate());
		default:
			throw std::string("RoseConstructLevelPredicates: unknown trait type");
	}
}

}	// namespace RoseConstructLevelPredicates

std::string RoseConstruct::getIdentifier() const {
	std::string identifier("");

	if (isSgLocatedNode(node) != nullptr) {
		auto locatedNode = isSgLocatedNode(node);
		logIt(DEBUG) << "getIdentifier(): \nNode type: " << locatedNode->class_name() << ":= ";

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
	// If code comes from template instantiation
	if (Utility::ASTHelper::isContainedInTemplateInstantiation(node)){
		logIt(DEBUG) << "Handling line info inside template instantiation for node " << node->class_name() << std::endl;
		return Utility::ASTHelper::applyConsumerToTemplateInstantiationDecl(
				[](Sg_File_Info *info) { return info->get_line(); }, node);
	}
	
	// Empty increment expression in for loop gets correct line info
	if (isSgNullExpression(node) && isSgForStatement(node->get_parent())) {
		return isSgLocatedNode(node->get_parent())->get_startOfConstruct()->get_line();
	}

	return isSgLocatedNode(node)->get_startOfConstruct()->get_line();
}

std::string RoseConstruct::determineCorrectFilename() const {
	// If code comes from template instantiation
	if (Utility::ASTHelper::isContainedInTemplateInstantiation(node)){
		logIt(DEBUG) << "Handling filename info inside template instantiation for node " << node->class_name() << std::endl;
		return Utility::ASTHelper::applyConsumerToTemplateInstantiationDecl(
				[](Sg_File_Info *info) { return info->get_filenameString(); }, node);
	}

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
	// If code comes from template instantiation
	if (Utility::ASTHelper::isContainedInTemplateInstantiation(node)) {
		logIt(DEBUG) << "Handling column info inside template instantiation for node " << node->class_name() << std::endl;
		return Utility::ASTHelper::applyConsumerToTemplateInstantiationDecl(
				[](Sg_File_Info *info) { return info->get_col(); }, node);
	}

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

int RoseFragment::determineCorrectLineInfo() const {
	return info->get_line();
}

int RoseFragment::determineCorrectColumnInformation() const {
	return info->get_col();
}

}	// namespace Core
}	// namespace Rose
}	// namespace InstRO
