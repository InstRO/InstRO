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
	if (current == nullptr) {
		std::cout << "raiseConstruct:\t terminated at NULL" << std::endl;
		return nullptr;
	}

	std::cout << "raiseConstruct: raising ended with " << current->class_name() << std::endl;
	// geth the corresponding construct from the RoseConstructProvider
	return InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(current);
}

template <class Predicate>
std::set<std::shared_ptr<InstRO::Rose::Core::RoseConstruct> > lowerConstruct(InstRO::Rose::Core::RoseConstruct *src,
																																						 Predicate pred) {
	std::set<std::shared_ptr<InstRO::Rose::Core::RoseConstruct> > retSet;
	// rose-query sub-tree with the right selectors
	Rose_STL_Container<SgNode *> nodes = SageInterface::querySubTree<SgNode>(src->getNode(), V_SgNode);
	for (auto node : nodes) {
		// check if the node is of the desired type ... and add it to the outputset
		if (pred(node)) {
			retSet.insert(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
		}
	}

	return retSet;
}

}	// namespace ConstructElevatorHelper

using namespace ConstructElevatorHelper;
using namespace InstRO::Rose::Core::RoseConstructLevelPredicates;

InstRO::Core::ConstructSet ConstructElevator::raise(const InstRO::Core::ConstructSet &inputCS,
																										InstRO::Core::ConstructTraitType traitType) {
	InstRO::InfracstructureInterface::ReadOnlyConstructSetCompilerInterface input(&inputCS);
	std::unique_ptr<InstRO::Core::ConstructSet> newConstructSet = std::make_unique<InstRO::Core::ConstructSet>();
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface output(newConstructSet.get());
	std::cout << "ConstructElevator::raise:\t Input-ConstructSet contains " << inputCS.size() << "elements " << std::endl;
	for (auto construct = input.cbegin(); construct != input.cend(); construct++) {
		std::shared_ptr<InstRO::Rose::Core::RoseConstruct> newConstruct;
		InstRO::Rose::Core::RoseConstruct *roseConstruct =
				dynamic_cast<InstRO::Rose::Core::RoseConstruct *>(construct->get());
		if (roseConstruct == nullptr) {
			throw std::string(
					"A non InstRO::Rose::Core::RoseConstruct in the ROSE interace. Either multiple compiler interfaces are used, "
					"or programming error");
		}

		switch (traitType) {
			case InstRO::Core::ConstructTraitType::CTExpression:
				newConstruct = raiseConstruct(roseConstruct, CLExpressionPredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTStatement:
				newConstruct = raiseConstruct(roseConstruct, CLStatementPredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTLoopStatement:
				newConstruct = raiseConstruct(roseConstruct, CLLoopPredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTConditionalStatement:
				newConstruct = raiseConstruct(roseConstruct, CLConditionalPredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTScopeStatement:
				newConstruct = raiseConstruct(roseConstruct, CLScopeStatementPredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTSimpleStatement:
				newConstruct = raiseConstruct(roseConstruct, CLSimpleStatementPredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTWrappableStatement:
				newConstruct = raiseConstruct(roseConstruct, CTWrappableStatementPredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTFunction:
				newConstruct = raiseConstruct(roseConstruct, CLFunctionPredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTFileScope:
				newConstruct = raiseConstruct(roseConstruct, CLFileScopePredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTGlobalScope:
				newConstruct = raiseConstruct(roseConstruct, CLGlobalScopePredicate());
				break;
		}

		if (newConstruct != nullptr) {
			output.put(newConstruct);
		}
	}
	std::cout << "ConstructElevator::raise:\t ConstructSet contains " << newConstructSet->size() << " elements "
						<< std::endl;
	return *newConstructSet;
};
InstRO::Core::ConstructSet ConstructElevator::lower(const InstRO::Core::ConstructSet *inputCS,
																										InstRO::Core::ConstructTraitType traitType) {
	return lower(*inputCS, traitType);
}
InstRO::Core::ConstructSet ConstructElevator::raise(const InstRO::Core::ConstructSet *inputCS,
																										InstRO::Core::ConstructTraitType traitType) {
	return raise(*inputCS, traitType);
}

// This is an explicit function used in very rare circumstances by e.g. a specialized selection pass (if at all)
InstRO::Core::ConstructSet ConstructElevator::lower(const InstRO::Core::ConstructSet &inputCS,
																										InstRO::Core::ConstructTraitType traitType) {
	std::unique_ptr<InstRO::Core::ConstructSet> newConstructSet = std::make_unique<InstRO::Core::ConstructSet>();
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface output(newConstructSet.get());

	// CI: check each input construct separately
	InstRO::InfracstructureInterface::ReadOnlyConstructSetCompilerInterface input(&inputCS);
	for (auto construct = input.cbegin(); construct != input.cend(); construct++) {
		std::set<std::shared_ptr<InstRO::Rose::Core::RoseConstruct> > newConstructs;
		// CI: make sure it is a ROSE construct
		auto roseConstruct = dynamic_cast<InstRO::Rose::Core::RoseConstruct *>(construct->get());
		if (roseConstruct == nullptr) {
			throw std::string(
					"A non InstRO::Rose::Core::RoseConstruct in the ROSE interace. Either multiple compiler interfaces are used, "
					"or programming error");
		}

		// Use different target predicate, depending on the input level
		switch (traitType) {
			case InstRO::Core::ConstructTraitType::CTExpression:
				newConstructs = lowerConstruct(roseConstruct, CLExpressionPredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTStatement:
				newConstructs = lowerConstruct(roseConstruct, CLStatementPredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTLoopStatement:
				newConstructs = lowerConstruct(roseConstruct, CLLoopPredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTConditionalStatement:
				newConstructs = lowerConstruct(roseConstruct, CLConditionalPredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTScopeStatement:
				newConstructs = lowerConstruct(roseConstruct, CLScopeStatementPredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTSimpleStatement:
				newConstructs = lowerConstruct(roseConstruct, CLSimpleStatementPredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTFunction:
				newConstructs = lowerConstruct(roseConstruct, CLFunctionPredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTFileScope:
				newConstructs = lowerConstruct(roseConstruct, CLFileScopePredicate());
				break;
			case InstRO::Core::ConstructTraitType::CTGlobalScope:
				newConstructs = lowerConstruct(roseConstruct, CLGlobalScopePredicate());
				break;
		}

		for (auto newConstruct : newConstructs) {
			output.put(newConstruct);
		}
	}
	return *newConstructSet;
}
}
}
}
}
