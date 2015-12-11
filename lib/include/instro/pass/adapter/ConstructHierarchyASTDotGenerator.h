#ifndef INSTRO_PASS_CONSTRUCT_HIERARCHY_AST_DOT_GENERATOR_H
#define INSTRO_PASS_CONSTRUCT_HIERARCHY_AST_DOT_GENERATOR_H

#include <fstream>
#include <string>
#include "instro/core/Instrumentor.h"
#include "instro/core/Singleton.h"
#include "instro/core/Pass.h"
#include "instro/core/ConstructSet.h"
#include "instro/tooling/AnalysisInterface.h"

namespace InstRO {
namespace Adapter {

/**
 * \brief Just a visualization for the construct hierarchy.
 */
class ConstructHierarchyASTDotGenerator : public InstRO::Core::PassImplementation {
 protected:
	InstRO::Pass *inputPass;
	std::string fileName;
	std::fstream outFile;

 protected:
	virtual std::string constructToString(std::shared_ptr<InstRO::Core::Construct> construct) { return std::string(""); }

 public:
	ConstructHierarchyASTDotGenerator(InstRO::Pass *pass, std::string filename)
			: PassImplementation(InstRO::Core::ChannelConfiguration(pass)), inputPass(pass), fileName(filename) {}
	virtual void init() override { outFile.open(fileName, std::ios_base::out); };
	virtual void execute() {
		outFile << "digraph InstROAST{" << std::endl;
		auto elevator = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator();
		InstRO::Core::ConstructSet csAggregation, workList;
		workList = *(inputPass->getOutput());

		while (workList.size()){
			InstRO::Core::ConstructSet  toDoList;
			InstRO::InfrastructureInterface::ConstructSetCompilerInterface csci(&workList);
			for (auto construct : csci) {
				auto child = construct;
				auto parent = child;

				InstRO::Core::ConstructSet childCS;
				InstRO::InfrastructureInterface::ConstructSetCompilerInterface(&childCS).put(construct);

				std::vector<InstRO::Core::ConstructTraitType> traitList;

				if (child->getTraits() == InstRO::Core::ConstructTraitType::CTExpression){
					// For Simple, Scope, Loop and Conditional elevate and add to process list
					traitList.push_back(InstRO::Core::ConstructTraitType::CTLoopStatement);
					traitList.push_back(InstRO::Core::ConstructTraitType::CTScopeStatement);
					traitList.push_back(InstRO::Core::ConstructTraitType::CTConditionalStatement);
					traitList.push_back(InstRO::Core::ConstructTraitType::CTSimpleStatement);
					traitList.push_back(InstRO::Core::ConstructTraitType::CTWrappableStatement);

				}
				else if (child->getTraits() == InstRO::Core::ConstructTraitType::CTStatement)
					traitList.push_back(InstRO::Core::ConstructTraitType::CTFunction);
				else if (child->getTraits() == InstRO::Core::ConstructTraitType::CTFunction)
					traitList.push_back(InstRO::Core::ConstructTraitType::CTFileScope);
				else if (child->getTraits() == InstRO::Core::ConstructTraitType::CTFileScope)
					traitList.push_back(InstRO::Core::ConstructTraitType::CTGlobalScope);
				else if (child->getTraits() == InstRO::Core::ConstructTraitType::CTGlobalScope) {}



				for (auto constructTrait : traitList) {
					auto parentCS = elevator->raise(childCS, constructTrait);
					InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface rocsciChild(&childCS);
					InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface rocsciParent(&parentCS);
					// if there is no partent continue
					if (parentCS.empty())
						continue;
					if (rocsciChild.size() != 1 || rocsciParent.size() != 1)
						throw std::string("Problem in ConstructHierarchyASTDotGenerator");
					outFile << "\t" << rocsciChild.begin()->get()->getID() << " -> " << rocsciParent.begin()->get()->getID()
						<< ";\n";
					// add the discoreved node to the processing list
					csAggregation = csAggregation.combine(parentCS);
					toDoList=toDoList.combine(parentCS);
				}
			}
			workList = toDoList;
		}
		csAggregation = csAggregation.combine(*inputPass->getOutput());
		auto csci = InstRO::InfrastructureInterface::ConstructSetCompilerInterface(&csAggregation);
		for (auto construct : csci) {
			outFile << "\t" << construct->getID() << std::string("[label=\"") << construct->toDotString()
							<< std::string("\"];") << std::endl;
		}

		outFile << "}" << std::endl;
	}
	virtual void finalize() override { outFile.close(); };
};

}
}

#endif
