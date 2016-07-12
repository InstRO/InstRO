#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/core/RoseConstructPredicates.h"

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

namespace RoseConstructTraitPredicates {

using namespace RoseConstructTraitPredicates;
std::unique_ptr<CTPredicate> getPredicateForTraitType(InstRO::Core::ConstructTraitType traitType) {
	switch (traitType) {
		case InstRO::Core::ConstructTraitType::CTExpression:
			return std::make_unique<CTExpressionPredicate>();
		case InstRO::Core::ConstructTraitType::CTStatement:
			return std::make_unique<CTStatementPredicate>();
		case InstRO::Core::ConstructTraitType::CTOpenMPStatement:
			return std::make_unique<CTOpenMPPredicate>();
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
			throw std::string("RoseConstructTraitPredicates: unknown trait type");
	}
}

}	// namespace RoseConstructTraitPredicates

std::string RoseConstruct::toString() const {
	std::string className = node->class_name();
	std::string unparse;

	if (getTraits().is(InstRO::Core::ConstructTraitType::CTFunction)) {
		// get only name of function
		unparse = isSgFunctionDefinition(node)->get_declaration()->get_qualified_name().getString();
	} else if (getTraits().is(InstRO::Core::ConstructTraitType::CTFileScope)) {
		// get only name of file
		unparse = isSgSourceFile(node)->getFileName();
	} else if (getTraits().is(InstRO::Core::ConstructTraitType::CTGlobalScope)) {
		unparse = std::string("GlobalScope");
	} else {
		unparse = node->unparseToString();
		// escape " and \n (e.g. in string literals)
	}
	return className + std::string(" | ") + unparse;
}

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
		return Utility::ASTHelper::applyConsumerToAssignInitializer(
				[](SgInitializedName *n) { return n->get_startOfConstruct()->get_line(); }, isSgAssignInitializer(node));
	}

	// if the declaration is inside an if statement, then it is marked compiler generated
	if (isSgVariableDeclaration(node)) {
		if (Sg_File_Info *fi = Utility::ASTHelper::getFileInfoFromWhileOrIf(node->get_parent())) {
			return fi->get_line();
		}
	}

	// This should be implicit casts (maybe even inserted at the front-end stage)
	if (isSgCastExp(node) && isSgCastExp(node)->isCompilerGenerated()) {
		return isSgLocatedNode(node->get_parent())->get_startOfConstruct()->get_line();
	}

	// compiler generated member function reference expressions for operator overloading
	if(isSgFunctionCallExp(node) && isSgFunctionCallExp(node)->isCompilerGenerated()){
		auto ste = SageInterface::querySubTree<SgVarRefExp>(isSgFunctionCallExp(node)->get_function());
		assert(ste.size() == 1 && "in the function call is referenced only one variable");
		return isSgLocatedNode(ste[0])->get_startOfConstruct()->get_line();
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
		if (Sg_File_Info *fi = Utility::ASTHelper::getFileInfoFromWhileOrIf(node->get_parent())) {
			return fi->get_filenameString();
		}
	}

	// Assign initializers are marked as compiler generated, s.a.
	if (isSgAssignInitializer(node)) {
		return Utility::ASTHelper::applyConsumerToAssignInitializer([](SgInitializedName *n) {
			return n->get_startOfConstruct()->get_filenameString();
		}, isSgAssignInitializer(node));
	}

	if (isSgCastExp(node) && isSgCastExp(node)->isCompilerGenerated()) {
		return isSgLocatedNode(node->get_parent())->get_startOfConstruct()->get_filenameString();
	}

	// compiler generated member function reference expressions for operator overloading
	if(isSgFunctionCallExp(node) && isSgFunctionCallExp(node)->isCompilerGenerated()){
		auto ste = SageInterface::querySubTree<SgVarRefExp>(isSgFunctionCallExp(node)->get_function());
		assert(ste.size() == 1 && "in the function call is referenced only one variable");
		return isSgLocatedNode(ste[0])->get_startOfConstruct()->get_filenameString();
	}

	return isSgLocatedNode(node)->get_startOfConstruct()->get_filenameString();
}

/*
 * As it seems this is overly complex, we do not retrieve the correct values in all cases.
 * Especially for something like: for(; ; ++i){..} we use the start column of the for loop as the
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
		if (Sg_File_Info *fi = Utility::ASTHelper::getFileInfoFromWhileOrIf(node->get_parent())) {
			return fi->get_col();
		}
	}

	// Assign initializers are marked as compiler generated, s.a.
	if (isSgAssignInitializer(node)) {
		return Utility::ASTHelper::applyConsumerToAssignInitializer([](SgInitializedName *n) {
			return n->get_endOfConstruct()->get_col() - (n->get_name().get_length() - 1);
		}, isSgAssignInitializer(node));
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

	// compiler generated member function reference expressions for operator overloading
	if(isSgFunctionCallExp(node) && isSgFunctionCallExp(node)->isCompilerGenerated()){
		auto ste = SageInterface::querySubTree<SgVarRefExp>(isSgFunctionCallExp(node)->get_function());
		assert(ste.size() == 1 && "in the function call is referenced only one variable");
		return isSgLocatedNode(ste[0])->get_startOfConstruct()->get_col();
	}

	return colInfo;
}

int RoseFragment::determineCorrectLineInfo() const { return info->get_line(); }

int RoseFragment::determineCorrectColumnInformation() const { return info->get_col(); }

std::shared_ptr<InstRO::Core::Construct> RoseConstructProvider::getFragment(SgNode* node, Sg_File_Info* fileInfo) {
	if (node == nullptr || fileInfo == nullptr) {
		throw std::string("RoseConstructProvider: attempted to getFragment for nullptr");
	}

	if (mapping.find(fileInfo) == mapping.end()) {
		logIt(DEBUG) << "\tcreating new construct" << std::endl;
		mapping[fileInfo] = std::make_shared<RoseFragment>(RoseFragment(node, fileInfo));
	}
	return mapping[fileInfo];
}

// global scope
void ConstructGenerator::visit(SgProject* node) {
	if (RoseConstructTraitPredicates::CTGlobalScopePredicate()(node)) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTGlobalScope);
	} else {
		generateError(node);
	}
}

// file scope
void ConstructGenerator::visit(SgSourceFile* node) {
	if (RoseConstructTraitPredicates::CTFileScopePredicate()(node)) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTFileScope);
	} else {
		generateError(node);
	}
}

// function
void ConstructGenerator::visit(SgFunctionDefinition* node) {
	if (RoseConstructTraitPredicates::CTFunctionPredicate()(node)) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTFunction);
	} else {
		generateError(node);
	}
}

// conditionals
void ConstructGenerator::visit(SgIfStmt* node) {
	ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTConditionalStatement);
	handleStatementWithWrappableCheck(node);
}
void ConstructGenerator::visit(SgSwitchStatement* node) {
	ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTConditionalStatement);
	handleStatementWithWrappableCheck(node);
}

// loops
void ConstructGenerator::visit(SgForStatement* node) {
	ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTLoopStatement);
	handleStatementWithWrappableCheck(node);
}
void ConstructGenerator::visit(SgWhileStmt* node) {
	ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTLoopStatement);
	handleStatementWithWrappableCheck(node);
}
void ConstructGenerator::visit(SgDoWhileStmt* node) {
	ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTLoopStatement);
	handleStatementWithWrappableCheck(node);
}

// scopes
void ConstructGenerator::visit(SgBasicBlock* node) {
	if (RoseConstructTraitPredicates::CTScopeStatementPredicate()(node)) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTScopeStatement);
		handleStatementWithWrappableCheck(node);
	} else {
		generateError(node);
	}
}

// statements
void ConstructGenerator::visit(SgStatement* node) {
	if (RoseConstructTraitPredicates::CTSimpleStatementPredicate()(node)) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTSimpleStatement);
		handleStatementWithWrappableCheck(node);
	} else {
		generateError(node);
	}
}

void ConstructGenerator::visit(SgVariableDeclaration* node) {
	// CI: an initialized variable declaration is OK,
	if (RoseConstructTraitPredicates::DefinedVariableDeclarationPredicate()(node)) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTSimpleStatement);
		handleStatementWithWrappableCheck(node);
	} else {
		generateError(node);
	}
}

// openmp
void ConstructGenerator::visit(SgOmpBarrierStatement* node) {
	if (RoseConstructTraitPredicates::CTOpenMPPredicate()(node)) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTOpenMPStatement);
		handleStatementWithWrappableCheck(node);
	} else {
		generateError(node);
	}
}

void ConstructGenerator::visit(SgOmpBodyStatement* node) {
	if (RoseConstructTraitPredicates::CTOpenMPPredicate()(node)) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTOpenMPStatement);
		handleStatementWithWrappableCheck(node);
	} else {
		generateError(node);
	}
}

void ConstructGenerator::visit(SgOmpFlushStatement* node) {
	if (RoseConstructTraitPredicates::CTOpenMPPredicate()(node)) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTOpenMPStatement);
		handleStatementWithWrappableCheck(node);
	} else {
		generateError(node);
	}
}

void ConstructGenerator::visit(SgOmpTaskwaitStatement* node) {
	if (RoseConstructTraitPredicates::CTOpenMPPredicate()(node)) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTOpenMPStatement);
		handleStatementWithWrappableCheck(node);
	} else {
		generateError(node);
	}
}

// expressions
void ConstructGenerator::visit(SgExpression* node) {
	if (RoseConstructTraitPredicates::CTExpressionPredicate()(node)) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTExpression);
	} else {
		generateError(node);
	}
}
void ConstructGenerator::handleStatementWithWrappableCheck(SgNode* node) {
	ct.add(InstRO::Core::ConstructTraitType::CTStatement);
	if (RoseConstructTraitPredicates::CTWrappableStatementPredicate()(node)) {
		ct.add(InstRO::Core::ConstructTraitType::CTWrappableStatement);
	}
}

void ConstructGenerator::generateError(SgNode* node) {
	ct = InstRO::Core::ConstructTraitType::CTNoTraits;
	logIt(INFO) << "ConstructGenerator: Skipped SgNode " << node->class_name() << "\t" << node->unparseToString()
							<< std::endl;
}

std::shared_ptr<InstRO::Core::Construct> RoseConstructProvider::getConstruct(SgNode* const node) {
	if (node == nullptr) {
		throw std::string("RoseConstructProvider: attempted to getConstruct for nullptr");
	}

	if (mapping.find(node) == mapping.end()) {
		logIt(DEBUG) << "\tcreating new construct" << std::endl;

		ConstructGenerator gen;
		node->accept(gen);

		if (gen.getConstructTraits().is(InstRO::Core::ConstructTraitType::CTNoTraits)) {
			mapping[node] = InstRO::Core::DummyConstruct::getInstance();
		} else {
			mapping[node] = std::make_shared<RoseConstruct>(RoseConstruct(node, gen.getConstructTraits()));
		}
	}
	return mapping[node];
}

}	// namespace Core
}	// namespace Rose
}	// namespace InstRO
