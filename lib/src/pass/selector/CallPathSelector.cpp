#include <memory>
#include <set>
#include "instro/core/Singleton.h"
#include "instro/pass/selector/CallPathSelector.h"
#include "instro/tooling/AnalysisInterface.h"

namespace InstRO {
namespace Selectors {

void CallPathSelector::init() {}
void CallPathSelector::execute() {
	// InstRO::Core::ConstructSet * fromCS, *toCS;

	// auto fromCS = std::make_shared<InstRO::Core::ConstructSet> (this->getInput(fromPass));
	// auto toCS = std::make_shared<InstRO::Core::ConstructSet>(this->getInput(toPass));
	auto fromCS = this->getInput(fromPass);
	auto toCS = this->getInput(toPass);
	// toCS = this->getInput(toPass);
	InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph *ecg =
			getInstrumentorInstance()->getAnalysisManager()->getECG();
	auto fromNodes = ecg->findNodes(fromCS);
	auto toNodes = ecg->findNodes(toCS);

	// for each start node trace the tree until we either have found an to node (in which case we add the nodes on the pat
	// to the output CS
	for (auto startNode = fromNodes.begin(); startNode != fromNodes.end(); startNode++) {
		for (auto toNode = toNodes.begin(); toNode != toNodes.end(); toNode++) {
			throw std::string("Not (Yet) Implemented");
			/*		// search for a path
					std::set< decltype(* toNode) > visitedNodes;
					// do the magic

					// add the found elements to the ouput bucket
					for (auto setIter = visitedNodes.begin(); setIter != visitedNodes.end(); setIter++)
						output.add((*setIter)->getCS());*/
		}
	}
	// for (InstRO::Core::ConstructSet * csInstance=)
}
void CallPathSelector::finalize(){

};
void CallPathSelector::releaseOutput(){
		//	output.clear();
};
Core::ConstructSet *CallPathSelector::getOutput() { return NULL; };

}	// Selector
}	// InstRO
