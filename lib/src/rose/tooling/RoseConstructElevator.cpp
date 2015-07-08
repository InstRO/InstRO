#include <memory>
#include <iostream>
#include "instro/core/Helper.h"

#include "instro/core/ConstructSet.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/tooling/RoseConstructElevator.h"

#include "rose.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace ConstructElevator {
namespace ConstructElevatorHelper {

template <class Predicate>
std::shared_ptr<InstRO::Rose::Core::RoseConstruct> raiseConstruct(InstRO::Rose::Core::RoseConstruct *src,
																																	Predicate pred) {
	SgNode *current = src->getNode();
	// CI Walk the AST upwards, until the current node is either NULL or it is the desired construct type
	while (current != nullptr && !pred(current)) {
		std::cout << "raiseConstruct:\t" << current->class_name() << std::endl;
		current = current->get_parent();
	}
	if (current == nullptr)
		std::cout << "raiseConstruct:\t terminated at NULL" << std::endl;
	else
		std::cout << "raiseConstruct: raising ended with " << current->class_name() << std::endl;
	// geth the corresponding construct from the RoseConstructProvider
	return InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(current);
}

template <class Predicate>
std::set<std::shared_ptr<InstRO::Rose::Core::RoseConstruct> > lowerConstruct(InstRO::Rose::Core::RoseConstruct *src,
																																						 Predicate pred) {
	std::set<std::shared_ptr<InstRO::Rose::Core::RoseConstruct> > retSet;
	SgNode *current = src->getNode();
	// rose-query sub-tree with the right selectors
	Rose_STL_Container<SgNode *> nodes = SageInterface::querySubTree<SgNode>(current, V_SgNode);
	for (auto node : nodes) {
		// check if the node is of the desired type ... and add it to the outputset
		if (pred(node))
			retSet.insert(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
	}

	return retSet;
}
}
using namespace ConstructElevatorHelper;

std::unique_ptr<InstRO::Core::ConstructSet> ConstructElevator::raise(InstRO::Core::ConstructSet *inputCS,
																																		 InstRO::Core::ConstructLevelType cl) {
	InstRO::Rose::Core::RoseConstruct *roseConstruct;
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface input(inputCS);
	std::unique_ptr<InstRO::Core::ConstructSet> newConstructSet = std::make_unique<InstRO::Core::ConstructSet>();
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface output(newConstructSet.get());
	std::cout << "ConstructElevator::raise:\t Input-ConstructSet contains " << inputCS->size() << "elements "
						<< std::endl;
	for (auto construct : input) {
		std::shared_ptr<InstRO::Rose::Core::RoseConstruct> newConstruct;
		roseConstruct = dynamic_cast<InstRO::Rose::Core::RoseConstruct *>(construct.get());
		if (roseConstruct == nullptr)
			throw std::string(
					"A non InstRO::Rose::Core::RoseConstruct in the ROSE interace. Either multiple compiler interfaces are used, "
					"or programming error");
		switch (cl) {
			case InstRO::Core::ConstructLevelType::CLExpression:
				newConstruct =
						raiseConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLExpressionPredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLStatement:
				newConstruct =
						raiseConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLStatementPredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLLoopStatement:
				newConstruct =
						raiseConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLLoopPredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLConditionalStatement:
				newConstruct =
						raiseConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLConditionalPredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLScopeStatement:
				newConstruct =
						raiseConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLScopePredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLSimpleStatement:
				newConstruct =
						raiseConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLSimplePredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLFunction:
				newConstruct =
						raiseConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLFunctionPredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLFileScope:
				newConstruct =
						raiseConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLFileScopePredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLGlobalScope:
				newConstruct =
						raiseConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLGlobalScopePredicate());
				break;
		}
		if (newConstruct->getLevel() != InstRO::Core::ConstructLevelType::CLNotALevel && newConstruct->getNode() != nullptr)
			output.put(newConstruct);
	}
	std::cout << "ConstructElevator::raise:\t ConstructSet contains " << newConstructSet->size() << "elements "
						<< std::endl;
	return newConstructSet;
};

// This is an explicit function used in very rare circumstances by e.g. a specialized selection pass (if at all)
std::unique_ptr<InstRO::Core::ConstructSet> ConstructElevator::lower(InstRO::Core::ConstructSet *inputCS,
																																		 InstRO::Core::ConstructLevelType cl) {
	InstRO::Rose::Core::RoseConstruct *roseConstruct;
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface input(inputCS);
	std::unique_ptr<InstRO::Core::ConstructSet> newConstructSet = std::make_unique<InstRO::Core::ConstructSet>();
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface output(newConstructSet.get());

	// CI: check each input construct seperately
	for (auto construct : input) {
		std::set<std::shared_ptr<InstRO::Rose::Core::RoseConstruct> > newConstructs;
		// CI: make sure it is a ROSE construct
		roseConstruct = dynamic_cast<InstRO::Rose::Core::RoseConstruct *>(construct.get());
		if (roseConstruct == nullptr)
			throw std::string(
					"A non InstRO::Rose::Core::RoseConstruct in the ROSE interace. Either multiple compiler interfaces are used, "
					"or programming error");
		// Usea different target predicate, depending on the input level
		switch (cl) {
			case InstRO::Core::ConstructLevelType::CLExpression:
				newConstructs =
						lowerConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLExpressionPredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLStatement:
				newConstructs =
						lowerConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLStatementPredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLLoopStatement:
				newConstructs =
						lowerConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLLoopPredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLConditionalStatement:
				newConstructs =
						lowerConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLConditionalPredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLScopeStatement:
				newConstructs =
						lowerConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLScopePredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLSimpleStatement:
				newConstructs =
						lowerConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLSimplePredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLFunction:
				newConstructs =
						lowerConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLFunctionPredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLFileScope:
				newConstructs =
						lowerConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLFileScopePredicate());
				break;
			case InstRO::Core::ConstructLevelType::CLGlobalScope:
				newConstructs =
						lowerConstruct(roseConstruct, InstRO::Rose::Core::RoseConstructLevelPredicates::CLGlobalScopePredicate());
				break;
		}
		for (auto newConstruct : newConstructs)
			output.put(newConstruct);
	}
	return newConstructSet;
}
}
}
}
}
