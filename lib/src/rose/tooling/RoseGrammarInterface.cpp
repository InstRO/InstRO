#include <memory>
#include "instro/core/Helper.h"
#include "instro/tooling/GrammarInterface.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/tooling/RoseGrammarInterface.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace GrammarInterface {

std::unique_ptr<InstRO::Core::ConstructSet> RoseGrammarInterface::getConstructsByType(
		const InstRO::Tooling::GrammarInterface::GrammarTypesType &types) {
	std::unique_ptr<InstRO::Core::ConstructSet> retSet = std::make_unique<InstRO::Core::ConstructSet>();
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface csci(retSet.get());

	std::vector<SgNode *> nodes;
	switch (types) {
		case InstRO::Tooling::GrammarInterface::GrammarTypesType::For:
			nodes = NodeQuery::querySubTree(proj, V_SgForStatement);
			break;
		default:
			break;
	}
	for (auto node : nodes) {	// std::vector<SgNode*>::iterator it = nodes.begin(); it != nodes.end(); it++){
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
	}
	return retSet;
}

std::list<InstRO::Tooling::GrammarInterface::GrammarTypesType> RoseGrammarInterface::getGrammerTypes(
		const InstRO::Core::ConstructSet &cs) {
	std::list<InstRO::Tooling::GrammarInterface::GrammarTypesType> retList;
	InstRO::Core::ConstructSet myCS = cs;
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface csci(&myCS);
	for (auto construct : csci) {
	}
	return retList;
}
}
}
}
}