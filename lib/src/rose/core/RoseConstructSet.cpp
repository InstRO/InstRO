#include "instro/rose/core/RoseConstructSet.h"

#include <cctype>

#include "instro/utility/MemoryManagement.h"
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
			return std::make_unique<CTExpressionPredicate>();
		case InstRO::Core::ConstructTraitType::CTStatement:
			return std::make_unique<CTStatementPredicate>();
		case InstRO::Core::ConstructTraitType::CTLoopStatement:
			return std::make_unique<CTLoopPredicate>();
		case InstRO::Core::ConstructTraitType::CTConditionalStatement:
			return std::make_unique<CTConditionalPredicate>();
		case InstRO::Core::ConstructTraitType::CTScopeStatement:
			return std::make_unique<CTScopeStatementPredicate>();
		case InstRO::Core::ConstructTraitType::CTSimpleStatement:
			return std::make_unique<CTSimpleStatementPredicate>();
		case InstRO::Core::ConstructTraitType::CTWrappableStatement:
			return std::make_unique<CTWrappableStatementPredicate>();
		case InstRO::Core::ConstructTraitType::CTFunction:
			return std::make_unique<CTFunctionPredicate>();
		case InstRO::Core::ConstructTraitType::CTFileScope:
			return std::make_unique<CTFileScopePredicate>();
		case InstRO::Core::ConstructTraitType::CTGlobalScope:
			return std::make_unique<CTGlobalScopePredicate>();
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
		std::string constructString(specificConstructTraitToString());
		std::string columnInfo(std::to_string(determineCorrectColumnInformation()));

		identifier += filename + ":" + lineInfo + ":";
		identifier += columnInfo + "--" + constructString;

		if (isSgFunctionDefinition(node)){
			auto fDecl = isSgFunctionDefinition(node)->get_declaration();
			identifier += "-" + fDecl->get_qualified_name().getString();
			identifier.erase(std::remove_if(identifier.begin(), identifier.end(), ::isspace), identifier.end());
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

	// Assign initializers are marked as compiler generated -> retrieve correct position information
	if (isSgAssignInitializer(node)) {
		return Utility::ASTHelper::applyConsumerToAssignInitializer([](Sg_File_Info *info) { return info->get_line(); },
																																isSgAssignInitializer(node));
	}

	// if the declaration is inside an if statement, then it is marked compiler generated
	if (isSgVariableDeclaration(node)) {
		if (isSgIfStmt(node->get_parent())) {
			return isSgIfStmt(node->get_parent())->get_startOfConstruct()->get_line();
		}
		if (isSgWhileStmt(node->get_parent())) {
			return isSgWhileStmt(node->get_parent())->get_startOfConstruct()->get_line();
		}
	}

	if (isSgCastExp(node) && isSgCastExp(node)->isCompilerGenerated()) {
		return isSgLocatedNode(node->get_parent())->get_startOfConstruct()->get_line();
	}

	return isSgLocatedNode(node)->get_startOfConstruct()->get_line();
}

std::string RoseConstruct::determineCorrectFilename() const {
	// If code comes from template instantiation
	if (Utility::ASTHelper::isContainedInTemplateInstantiation(node)){
		logIt(DEBUG) << "=== ROSE CONSTRUCT ===\nHandling filename info inside template instantiation for node " << node->class_name() << std::endl;
		logIt(DEBUG) << "====\n" << toString() << "\n ====" << std::endl;
		return Utility::ASTHelper::applyConsumerToTemplateInstantiationDecl(
				[](Sg_File_Info *info) { return info->get_filenameString(); }, node);
	}

	// empty increment expression in for loop gets correct file name
	if(isSgNullExpression(node) && isSgForStatement(node->get_parent())){
		return isSgLocatedNode(node->get_parent())->get_startOfConstruct()->get_filenameString();
	}

	// if the declaration is inside an if statement, then it is marked compiler generated
	if (isSgVariableDeclaration(node)) {
		if (isSgIfStmt(node->get_parent())) {
			return isSgIfStmt(node->get_parent())->get_startOfConstruct()->get_filenameString();
		}
		if (isSgWhileStmt(node->get_parent())) {
			return isSgWhileStmt(node->get_parent())->get_startOfConstruct()->get_filenameString();
		}
	}

	// Assign initializers are marked as compiler generated, s.a.
	if (isSgAssignInitializer(node)) {
		return Utility::ASTHelper::applyConsumerToAssignInitializer(
				[](Sg_File_Info *info) { return info->get_filenameString(); }, isSgAssignInitializer(node));
	}

	if (isSgCastExp(node) && isSgCastExp(node)->isCompilerGenerated()) {
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

	// if the declaration is inside an if statement, then it is marked compiler generated
	if (isSgVariableDeclaration(node)) {
		if (isSgIfStmt(node->get_parent())) {
			return isSgIfStmt(node->get_parent())->get_startOfConstruct()->get_col();
		}
		if (isSgWhileStmt(node->get_parent())) {
			return isSgWhileStmt(node->get_parent())->get_startOfConstruct()->get_col();
		}
	}

	// Assign initializers are marked as compiler generated, s.a.
	if (isSgAssignInitializer(node)) {
		return Utility::ASTHelper::applyConsumerToAssignInitializer([](Sg_File_Info *info) { return info->get_col(); },
																																isSgAssignInitializer(node));
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

	if (isSgCastExp(node) && isSgCastExp(node)->isCompilerGenerated()) {
		colInfo = isSgLocatedNode(node->get_parent())->get_startOfConstruct()->get_col();
	}

	return colInfo;
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
