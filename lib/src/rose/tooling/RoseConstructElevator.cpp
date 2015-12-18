#include <memory>
#include "instro/core/Helper.h"

#include "instro/core/ConstructSet.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/tooling/RoseConstructElevator.h"

#include "instro/utility/Logger.h"

#include "rose.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace ConstructElevator {
namespace ConstructElevatorHelper {

template <class Predicate>
std::shared_ptr<InstRO::Core::Construct> raiseConstruct(std::shared_ptr<InstRO::Rose::Core::RoseConstruct> src,
																																	Predicate pred) {
	SgNode *current = src->getNode();
	// CI Walk the AST upwards, until the current node is either NULL or it is the desired construct type
	while (current != nullptr && !pred->operator()(current)) {
		logIt(INFO) << " --> " << current->class_name();
		current = current->get_parent();
	}
	if (current == nullptr) {
		logIt(INFO) << " --> NULL" << std::endl;
		return nullptr;
	}

	logIt(INFO) << "raiseConstruct: raising ended with " << current->class_name() << std::endl;
	// get the corresponding construct from the RoseConstructProvider
	return InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(current);
}

template <class Predicate>
std::set<std::shared_ptr<InstRO::Core::Construct> > lowerConstruct(std::shared_ptr<InstRO::Rose::Core::RoseConstruct> src,
																																						 Predicate pred) {
	std::set<std::shared_ptr<InstRO::Core::Construct> > retSet;
	// rose-query sub-tree with the right selectors
	Rose_STL_Container<SgNode *> nodes = SageInterface::querySubTree<SgNode>(src->getNode(), V_SgNode);
	for (auto node : nodes) {
		// check if the node is of the desired type ... and add it to the outputset
		if (pred->operator()(node)) {
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
	InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface input(&inputCS);
	InstRO::Core::ConstructSet newConstructSet;
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface output(&newConstructSet);
	logIt(INFO) << "ConstructElevator::raise to " << traitType << ":\t Input-ConstructSet contains " << inputCS.size()
						<< "elements " << std::endl;
	for (auto construct : input) {
		auto roseConstruct = InstRO::Rose::toRoseConstruct(construct);

		auto predicate = InstRO::Rose::Core::RoseConstructLevelPredicates::getPredicateForTraitType(traitType);
		std::shared_ptr<InstRO::Core::Construct> newConstruct = raiseConstruct(roseConstruct, std::move(predicate));

		if (newConstruct != nullptr) {
			output.put(newConstruct);
		}
	}
	logIt(INFO) << "ConstructElevator::raise:\t ConstructSet contains " << newConstructSet.size() << " elements "
						<< std::endl;
	return newConstructSet;
}

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
	InstRO::Core::ConstructSet newConstructSet;
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface output(&newConstructSet);

	// CI: check each input construct separately
	InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface input(&inputCS);
	for (auto construct : input) {
		// CI: make sure it is a ROSE construct
		auto roseConstruct = InstRO::Rose::toRoseConstruct(construct);
		auto predicate = InstRO::Rose::Core::RoseConstructLevelPredicates::getPredicateForTraitType(traitType);
		auto newConstructs = lowerConstruct(roseConstruct, std::move(predicate));

		for (auto newConstruct : newConstructs) {
			output.put(newConstruct);
		}
	}
	return newConstructSet;
}
}
}
}
}
