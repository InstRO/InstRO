#include <memory>
#include "instro/core/Helper.h"
#include "instro/tooling/GrammarInterface.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/tooling/RoseGrammarInterface.h"

#include "rose.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace GrammarInterface {

// CI: the following serves as a template how to provide grammer access for different constructs, here the For Construct
std::unique_ptr<InstRO::Core::ConstructSet> RoseGrammarInterface::getConstructsByType(
		const InstRO::Tooling::GrammarInterface::GrammarTypesType &type) {
	std::unique_ptr<InstRO::Core::ConstructSet> retSet = std::make_unique<InstRO::Core::ConstructSet>();
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface csci(retSet.get());

	std::vector<SgNode *> nodes, tmp;
	switch (type) {
		case InstRO::Tooling::GrammarInterface::GrammarTypesType::GTFor:
			nodes = NodeQuery::querySubTree(proj, V_SgForStatement);
			break;
		case InstRO::Tooling::GrammarInterface::GrammarTypesType::GTFunction:
			nodes = NodeQuery::querySubTree(proj, V_SgFunctionDefinition);
			tmp = NodeQuery::querySubTree(proj, V_SgTemplateFunctionDefinition);
			nodes.insert(nodes.end(), tmp.begin(), tmp.end());
			tmp.clear();
			break;

		default:
			throw std::string("RoseGrammarInterface::getConstructsByType for type (enum):") + std::to_string((int)(type)) +
					std::string(" not implemented\t") + std::string(__FILE__) + std::string(", line ") + std::to_string(__LINE__);
			break;
	}
	for (auto node : nodes) {
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
	}
	return retSet;
}

InstRO::Tooling::GrammarInterface::GrammarTypesType astNodeToGrammarType(SgNode *n) {
	if (n == nullptr)
		return InstRO::Tooling::GrammarInterface::GrammarTypesType::GTInvalid;
	else if (isSgForStatement(n) != nullptr)
		return InstRO::Tooling::GrammarInterface::GrammarTypesType::GTFor;
	else if (isSgFunctionDeclaration(n) != nullptr)
		return InstRO::Tooling::GrammarInterface::GrammarTypesType::GTFunction;
	else if (isSgFunctionDefinition(n) != nullptr)
		return InstRO::Tooling::GrammarInterface::GrammarTypesType::GTFunction;
	throw std::string("RoseGrammarInterface::astNodeToGrammarType node (SgNode):") + n->class_name() +
			std::string(" not implemented \t") + std::string(__FILE__) + std::string(", line ") + std::to_string(__LINE__);
	return InstRO::Tooling::GrammarInterface::GrammarTypesType::GTInvalid;
}

// * Return the different grammar types found in the construct set
std::list<InstRO::Tooling::GrammarInterface::GrammarTypesType> RoseGrammarInterface::getGrammerTypes(
		const InstRO::Core::ConstructSet &cs) {
	std::list<InstRO::Tooling::GrammarInterface::GrammarTypesType> retList;
	// This requires Low-Level Access to the Constructs
	InstRO::Core::ConstructSet myCS = cs;
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface csci(&myCS);
	// For each construct in the set ...
	for (auto construct : csci) {
		//	auto roseConstruct = std::dynamic_pointer_cast<InstRO::Rose::Core::RoseConstruct>(construct);

		// use the raw pointer of the construct to get a casted instance of the RoseConstruct since we need only the pointer
		// to the SgNode
		SgNode *node = dynamic_cast<InstRO::Rose::Core::RoseConstruct *>(construct.get())->getNode();
		InstRO::Tooling::GrammarInterface::GrammarTypesType constructType = astNodeToGrammarType(node);
		if (constructType == InstRO::Tooling::GrammarInterface::GrammarTypesType::GTInvalid)
			throw std::string("Invalid Grammar Type in ConstructSet") + node->unparseToString();
		retList.push_back(constructType);
	}
	return retList;
}	// getGrammerTypes

std::unique_ptr<InstRO::Core::ConstructSet> RoseGrammarInterface::getConstructsByClass(
		const InstRO::Core::ConstructTraitType constructClass) {

	std::unique_ptr<InstRO::Core::ConstructSet> result = std::make_unique<InstRO::Core::ConstructSet>();
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface csci(result.get());

	for (auto sgNode : SageInterface::querySubTree<SgNode>(proj, V_SgNode)) {
		auto construct = InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(sgNode);
		if (construct->getTraits().is(constructClass)) {
			csci.put(construct);
		}
	}

	return result;
}

}	// GrammarInterface
}	// Tooling
}	// Rose
}	// InstRO
