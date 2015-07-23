#include "rose.h"
#include "instro/rose/core/RoseConstructSet.h"

#include <fstream>
#include <string>


/*
std::string constructToString(SgNode* node) {
	// Since we are in a RoseInstRO it is safe to cast InstRO::Core::Construct to InstRO::Rose::Core::RoseConstruct
	//	auto roseConstruct = dynamic_cast<InstRO::Rose::Core::RoseConstruct &>(construct);

	auto roseConstruct = std::dynamic_pointer_cast<InstRO::Rose::Core::RoseConstruct>(construct);
	if (construct->getTraits() == InstRO::Core::ConstructTraitType::CTFunction)
		return isSgFunctionDefinition(roseConstruct->getNode())->get_declaration()->get_name();
	else if (construct->getTraits() == InstRO::Core::ConstructTraitType::CTFileScope)
		return isSgFile(roseConstruct->getNode())->getFileName();
	else if (construct->getTraits() == InstRO::Core::ConstructTraitType::CTGlobalScope)
		return std::string("");
	else
		return roseConstruct->getNode()->unparseToString();
}

*/
int main(int argc, char ** argv)
{
	auto project = frontend(argc, argv);

	std::fstream outFile;
	outFile.open("InstRORoseAST.dot", std::ios_base::out); 
	outFile << "digraph RoseInstROAST{" << std::endl;

	std::vector<SgNode*> sgNodes = NodeQuery::querySubTree(project, V_SgNode);
	for (auto node : sgNodes){
		if (node->get_file_info()==nullptr || node->get_file_info()->isCompilerGenerated ()) continue;
		SgNode * parent = node->get_parent();
		while (parent!=nullptr &&  (parent->get_file_info()==nullptr || node->get_file_info()->isCompilerGenerated ()))
			parent=parent->get_parent();
		// write the name of the node, and some syntactical represenation
		std::string roseClassName=node->class_name();
		
		InstRO::Rose::Core::ConstructGenerator cg;
		node->accept(cg);
		std::string instroConstructName = cg.getConstructTraits().toString();
		//(cg.getConstructTraits(node);
		std::string nodeToString;
		std::string nodeFormatArgs;
		if (InstRO::Rose::Core::RoseConstructLevelPredicates::ConstructPredicate()(node))
			nodeFormatArgs = ",shape=box";
		else
			nodeFormatArgs = ",shape=circle";
		
		std::replace(nodeFormatArgs.begin(), nodeFormatArgs.end(), '"', ' ');

		outFile << "\tn" << node << " [label=\"" << roseClassName << "\\n" << instroConstructName << "\\n" << nodeToString << "\"" << nodeFormatArgs << "];" << std::endl;
		if (parent != NULL)
		{
			outFile << "\tn" << node << " -> n" << parent <<";"<< std::endl;
		}

	}



	outFile << "}" << std::endl;
	outFile.close();
	/*
			auto elevator = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator();
			InstRO::Core::ConstructSet csAggregation;

			InstRO::InfracstructureInterface::ConstructSetCompilerInterface csci(inputPass->getOutput());
			for (auto construct : csci) {
				auto child = construct;
				auto parent = child;
				auto childCS = InstRO::Core::ConstructSet(construct);
				// ad the current node to the bucket
				csAggregation = csAggregation.combine(childCS);

				// for each construct level, bottom to top, try raising the construct
				for (auto constructTrait = InstRO::Core::ConstructTraitType::CTExpression;
					constructTrait != InstRO::Core::ConstructTraitType::CTMax; constructTrait++) {

					auto parentCS = elevator->raise(child, constructTrait);
					// if the node is in the bucket, we already have plottet it, so we can skip it (and its chain)
					if (!csAggregation.intersect(parentCS).empty())
						continue;

					InstRO::InfracstructureInterface::ReadOnlyConstructSetCompilerInterface rocsciChild(&childCS);
					InstRO::InfracstructureInterface::ReadOnlyConstructSetCompilerInterface rocsciParent(&parentCS);
					// if there is no partent continue
					if (parentCS.empty())
						continue;
					csAggregation = csAggregation.combine(parentCS);
					if (rocsciChild.size() != 1 || rocsciParent.size() != 1)
						throw std::string("Problem in ConstructHierarchyASTDotGenerator");


					outFile << "\t" << rocsciChild.cbegin()->get()->getID() << " -> " << rocsciParent.cbegin()->get()->getID()
						<< ";\n";

					childCS = parentCS;
				}
			}
			csci = InstRO::InfracstructureInterface::ConstructSetCompilerInterface(&csAggregation);
			for (auto construct : csci) {
				outFile << "\t" << construct->getID() << std::string("[label=\"") << constructToString(construct) << std::string("\"];") << std::endl;


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

					auto roseConstruct = std::dynamic_pointer_cast<InstRO::Rose::Core::RoseConstruct>(construct);
					if (construct->getTraits() == InstRO::Core::ConstructTraitType::CTFunction)
						return isSgFunctionDefinition(roseConstruct->getNode())->get_declaration()->get_name();
					else if (construct->getTraits() == InstRO::Core::ConstructTraitType::CTFileScope)
						return isSgFile(roseConstruct->getNode())->getFileName();
					else if (construct->getTraits() == InstRO::Core::ConstructTraitType::CTGlobalScope)
						return std::string("");
					else
						return roseConstruct->getNode()->unparseToString();
				}

			public:
				RoseConstructHierarchyASTDotGenerator(InstRO::Pass *pass, std::string fn)
					: ConstructHierarchyASTDotGenerator(pass, fn) {}
			};
		}
	}
}
#endif
*/


}
