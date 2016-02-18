#include "rose.h"
#include "instro/rose/core/RoseConstructSet.h"

#include <fstream>
#include <string>
#include <vector>
#include <map>


// http://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path
struct MatchPathSeparator
{
    bool operator()( char ch ) const
    {
        return ch == '/';
    }
};

std::string
basename( std::string const& pathname )
{
    return std::string( 
        std::find_if( pathname.rbegin(), pathname.rend(),
                      MatchPathSeparator() ).base(),
        pathname.end() );
}



std::string constructToString(SgNode* node) {
	// Since we are in a RoseInstRO it is safe to cast InstRO::Core::Construct to InstRO::Rose::Core::RoseConstruct
	//	auto roseConstruct = dynamic_cast<InstRO::Rose::Core::RoseConstruct &>(construct);

	if (isSgFunctionDefinition(node)!=nullptr)
		return isSgFunctionDefinition(node)->get_declaration()->get_name();
	if (isSgFunctionDeclaration(node)!=nullptr)
		return isSgFunctionDeclaration(node)->get_name();
	else if (isSgFile(node)!=nullptr)
		return basename(isSgFile(node)->getFileName());
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
	outFile << "graph [pad=\"0.1\", nodesep=\"0.3\", ranksep=\"0.3\"];\n# graph [pad=\"0.1\", nodesep=\"0.5\", ranksep=\"0.5\", splines=ortho];\n# splines=\"false\";\n#    node[shape = square];\n#    edge[style=invis];" << std::endl; 

	std::vector<SgNode*> nodes = NodeQuery::querySubTree(project, V_SgNode);
	std::vector<SgNode*> processing,toDoList,doneList;
	std::map<SgNode*,int> nodeToIDMap;
	for (auto node : nodes)
	{
		if (node->get_file_info()==nullptr || node->get_file_info()->isCompilerGenerated ()) continue;
		else toDoList.push_back(node);
	}


	int idCounter=0;
	while (toDoList.size())
	{
		processing=toDoList;
		toDoList.clear();
		for (auto node : processing ){
			// continue if we already processed that node
			if (std::find(doneList.begin(),doneList.end(),node)!=doneList.end())
				continue;
			doneList.push_back(node);
			nodeToIDMap[node]=idCounter;idCounter++;

			// gather the name of the node, and some syntactical represenation
			std::string roseClassName=node->class_name();
			// Remove Sg
                        roseClassName=roseClassName.erase(roseClassName.find("Sg"),std::string("Sg").length());

 
			InstRO::Rose::Core::ConstructGenerator cg;
			node->accept(cg);
			std::string instroConstructName = cg.getConstructTraits().toStringShort();

			//(cg.getConstructTraits(node);
			std::string nodeToString=constructToString(node);

			std::string nodeFormatArgs;
			if (InstRO::Rose::Core::RoseConstructTraitPredicates::ConstructPredicate()(node))
				nodeFormatArgs = ",shape=box,peripheries=2,style=filled, fillcolor=palegreen,fontsize=10,margin=0.02";
			else
				nodeFormatArgs = ",shape=box, style=filled,fillcolor=gray,fontsize=10,margin=0.02";// salmon

			// Make sure we don't have any stray " in our strings		
			std::replace(nodeFormatArgs.begin(), nodeFormatArgs.end(), '"', ' ');
			std::replace(nodeToString.begin(), nodeToString.end(), '"', ' ');

//			outFile << "\tn" << node << " [label=\"" << roseClassName << "\\n" << instroConstructName << "\\n" << nodeToString << "\"" << nodeFormatArgs << "];" << std::endl;
			outFile << "\tn" << node << " [label=\"" << roseClassName << "\\n" << nodeToString <<"\\nID " << nodeToIDMap[node] << "\"" << nodeFormatArgs << "];" << std::endl;

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
	// Create the Construct AST
	
	outFile.open("InstROConstructAST.dot", std::ios_base::out); 
	outFile << "digraph RoseConstructAST{" << std::endl;
	outFile << "graph [pad=\"0.1\", nodesep=\"0.3\", ranksep=\"0.3\"];\n# graph [pad=\"0.1\", nodesep=\"0.5\", ranksep=\"0.5\", splines=ortho];\n# splines=\"false\";\n#    node[shape = square];\n#    edge[style=invis];" << std::endl; 

	nodes = NodeQuery::querySubTree(project, V_SgNode);
	processing.clear();
	toDoList.clear();
	doneList.clear();
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
			// Remove Sg
                        roseClassName=roseClassName.erase(roseClassName.find("Sg"),std::string("Sg").length());

 
			InstRO::Rose::Core::ConstructGenerator cg;
			node->accept(cg);
			std::string instroConstructName = cg.getConstructTraits().toStringShort();

			//(cg.getConstructTraits(node);
			std::string nodeToString=constructToString(node);

			std::string nodeFormatArgs;
			if (InstRO::Rose::Core::RoseConstructTraitPredicates::ConstructPredicate()(node)) {
				nodeFormatArgs = ",shape=box,peripheries=2,style=filled, fillcolor=palegreen,fontsize=10,margin=0.02";
			// Make sure we don't have any stray " in our strings		
				std::replace(nodeFormatArgs.begin(), nodeFormatArgs.end(), '"', ' ');
				std::replace(nodeToString.begin(), nodeToString.end(), '"', ' ');

				outFile << "\tn" << node << " [label=\"" << instroConstructName << "\\nID " << nodeToIDMap[node] << "\"" << nodeFormatArgs << "];" << std::endl;
	//			outFile << "\tn" << node << " [label=\"" << roseClassName << "\\n" << nodeToString << "\"" << nodeFormatArgs << "];" << std::endl;
}
			// if the parent of the current node is compiler generated, get the next non-compiler generated node and uses it instead
			SgNode * parent = node->get_parent();
			while (parent != nullptr) {
//				if (!(parent->get_file_info()==nullptr || parent->get_file_info()->isCompilerGenerated ())) {
					// break with a non-null parent to create edge
					if (InstRO::Rose::Core::RoseConstructTraitPredicates::ConstructPredicate()(parent)) break;
//				}
				parent = parent->get_parent();					
			}
					
			if (parent == nullptr) continue;

//			if (parent->get_file_info()==nullptr || parent->get_file_info()->isCompilerGenerated ())
				toDoList.push_back(parent);

	//			outFile << "\tn" << node << " [label=\"" << roseClassName << "\\n" << nodeToString << "\"" << nodeFormatArgs << "];" << std::endl;
			if (InstRO::Rose::Core::RoseConstructTraitPredicates::ConstructPredicate()(node)) {
				outFile << "\tn" << node << " -> n" << parent <<";"<< std::endl;
			}
			

		}
	}

	outFile << "}" << std::endl;
	outFile.close();



}
