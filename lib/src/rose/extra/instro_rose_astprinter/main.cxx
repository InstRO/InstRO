#include "rose.h"
#include "instro/rose/core/RoseConstructSet.h"

#include <fstream>
#include <string>
#include <vector>
#include <map>


std::string constructToString(SgNode* node) {
	// Since we are in a RoseInstRO it is safe to cast InstRO::Core::Construct to InstRO::Rose::Core::RoseConstruct
	//	auto roseConstruct = dynamic_cast<InstRO::Rose::Core::RoseConstruct &>(construct);

	if (isSgFunctionDefinition(node)!=nullptr)
		return isSgFunctionDefinition(node)->get_declaration()->get_name();
	if (isSgFunctionDeclaration(node)!=nullptr)
		return isSgFunctionDeclaration(node)->get_name();
	else if (isSgFile(node)!=nullptr)
		return isSgFile(node)->getFileName();
	else if (isSgProject(node)!=nullptr)
		return std::string("");
	else if (isSgBasicBlock(node)!=nullptr)
		return std::string("{...}");
	else if (isSgForStatement(node)!= nullptr)
		return std::string("for(...)");
	else if (isSgIfStmt(node)!=nullptr)
		return std::string("if(...)");
	else if (isSgGlobal(node)!=nullptr)
		return std::string("");
	else
		return node->unparseToString();
}

int main(int argc, char ** argv)
{
	auto project = frontend(argc, argv);

	std::fstream outFile;
	outFile.open("InstRORoseAST.dot", std::ios_base::out); 
	outFile << "digraph RoseInstROAST{" << std::endl;

	std::vector<SgNode*> nodes = NodeQuery::querySubTree(project, V_SgNode);
	std::vector<SgNode*> processing,toDoList,doneList;
	for (auto node : nodes)
	{
		if (node->get_file_info()==nullptr || node->get_file_info()->isCompilerGenerated ()) continue;
		else toDoList.push_back(node);
	}


	while (toDoList.size())
	{
		processing=toDoList;
		toDoList.clear();
		for (auto node : processing ){
			// continue if we already processed that node
			if (std::find(doneList.begin(),doneList.end(),node)!=doneList.end())
				continue;
			doneList.push_back(node);

			// gather the name of the node, and some syntactical represenation
			std::string roseClassName=node->class_name();
			InstRO::Rose::Core::ConstructGenerator cg;
			node->accept(cg);
			std::string instroConstructName = cg.getConstructTraits().toStringShort();

			//(cg.getConstructTraits(node);
			std::string nodeToString=constructToString(node);

			std::string nodeFormatArgs;
			if (InstRO::Rose::Core::RoseConstructLevelPredicates::ConstructPredicate()(node))
				nodeFormatArgs = ",shape=box";
			else
				nodeFormatArgs = ",shape=hexagon";

			// Make sure we don't have any stray " in our strings		
			std::replace(nodeFormatArgs.begin(), nodeFormatArgs.end(), '"', ' ');
			std::replace(nodeToString.begin(), nodeToString.end(), '"', ' ');

			outFile << "\tn" << node << " [label=\"" << roseClassName << "\\n" << instroConstructName << "\\n" << nodeToString << "\"" << nodeFormatArgs << "];" << std::endl;

			// if the parent of the current node is compiler generated, get the next non-compiler generated node and uses it instead
			SgNode * parent = node->get_parent();
			if (parent == nullptr) continue;

			if (parent->get_file_info()==nullptr || parent->get_file_info()->isCompilerGenerated ())
				toDoList.push_back(parent);
			
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