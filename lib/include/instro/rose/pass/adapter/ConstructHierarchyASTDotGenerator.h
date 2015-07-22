#ifndef INSTRO_ROSE_RoseConstructHierarchyASTDotGenerator
#define INSTRO_ROSE_RoseConstructHierarchyASTDotGenerator

#include <fstream>
#include <algorithm>
#include <string>
#include "instro/core/Singleton.h"
#include "instro/core/Pass.h"
#include "instro/core/ConstructSet.h"
#include "instro/tooling/AnalysisInterface.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/core/RosePassImplementation.h"

namespace InstRO {
namespace Adapter {

class ConstructHierarchyASTDotGenerator : public InstRO::Core::PassImplementation {
 protected:
	InstRO::Pass *inputPass;
	InstRO::Core::ConstructSet outputCS;
	std::string fileName;
	std::fstream outFile;

 protected:
	virtual std::string constructToString(std::shared_ptr<InstRO::Core::Construct> construct) { return std::string(""); }

 public:
	ConstructHierarchyASTDotGenerator(InstRO::Pass *pass, std::string filename)
			: PassImplementation(InstRO::Core::ChannelConfiguration(pass)), inputPass(pass), fileName(filename) {}
	virtual void init() { outFile.open(fileName, std::ios_base::out); };
	virtual void execute() {
		outFile << "digraph InstROAST{" <<std::endl;
		auto elevator = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator();
		InstRO::Core::ConstructSet csAggregation;

		InstRO::InfracstructureInterface::ConstructSetCompilerInterface csci(inputPass->getOutput());
		for (auto construct : csci) {
			auto child = construct;
			auto parent = child;
			auto childCS = InstRO::Core::ConstructSet(construct);

			// for each construct level, bottom to top, try raising the construct
			for (auto constructTrait = InstRO::Core::ConstructTraitType::CTExpression;
					 constructTrait != InstRO::Core::ConstructTraitType::CTMax; constructTrait++) {
				csAggregation = csAggregation.combine(childCS);
				auto parentCS = elevator->raise(child, constructTrait);
				InstRO::InfracstructureInterface::ReadOnlyConstructSetCompilerInterface rocsciChild(&childCS);
				InstRO::InfracstructureInterface::ReadOnlyConstructSetCompilerInterface rocsciParent(&parentCS);
				// if there is no partent continue
				if (parentCS.empty())
					continue;

				if (rocsciChild.size() != 1 || rocsciParent.size() != 1)
					throw std::string("Problem in ConstructHierarchyASTDotGenerator");
				if (rocsciChild.cbegin()->get()->getID() == rocsciParent.cbegin()->get()->getID()) continue;
				outFile << "\t" << rocsciChild.cbegin()->get()->getID() << " -> " << rocsciParent.cbegin()->get()->getID()
								<< ";\n";

				childCS = parentCS;
			}

			/*			auto parent = elevator->raise(construct)
						std::cout << "\t" << construct->getID() << " -> " << std::endl;*/
		}
		csci = InstRO::InfracstructureInterface::ConstructSetCompilerInterface(&csAggregation);
		for (auto construct : csci) {
			std::string csName=constructToString(construct);
			std::replace(csName.begin(),csName.end(),'"',' ');
			outFile << "\t" << construct->getID() << std::string("[label=\"") << csName << std::string("\"];") << std::endl; 
			

		}

		outFile << "}" << std::endl;
	}
	virtual void finalize() { outFile.close(); };
	void releaseOutput() { outputCS.clear(); };
	InstRO::Core::ConstructSet *getOutput() { return &outputCS; }
};
}
}

namespace InstRO {
namespace Rose {
namespace Adapter {

class RoseConstructHierarchyASTDotGenerator : public InstRO::Adapter::ConstructHierarchyASTDotGenerator {
 protected:
	 virtual std::string constructToString(std::shared_ptr<InstRO::Core::Construct> construct) {
		// Since we are in a RoseInstRO it is safe to cast InstRO::Core::Construct to InstRO::Rose::Core::RoseConstruct
	//	auto roseConstruct = dynamic_cast<InstRO::Rose::Core::RoseConstruct &>(construct);
		std::string content, name;
		auto roseConstruct = std::dynamic_pointer_cast<InstRO::Rose::Core::RoseConstruct>(construct);
		if (construct->getTraits() == InstRO::Core::ConstructTraitType::CTFunction)
			content= isSgFunctionDefinition(roseConstruct->getNode())->get_declaration()->get_name();
		else if (construct->getTraits() == InstRO::Core::ConstructTraitType::CTFileScope)
			content=isSgFile(roseConstruct->getNode())->getFileName();
		else if (construct->getTraits() == InstRO::Core::ConstructTraitType::CTGlobalScope)
			content=std::string("");
		else
			content=roseConstruct->getNode()->unparseToString();
		name = roseConstruct->getNode()->class_name();
		return name+std::string("\n")+content;
	}

 public:
	RoseConstructHierarchyASTDotGenerator(InstRO::Pass *pass, std::string fn)
			: ConstructHierarchyASTDotGenerator(pass, fn) {}
};
}
}
}
#endif
