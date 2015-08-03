#include "rose.h"

#include "instro/tooling/ExtendedCallGraph.h"
#include "instro/rose/core/RoseConstructSet.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace ExtendedCallGraph {

using InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph;
using InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode;
using InstRO::Tooling::ExtendedCallGraph::ECGNodeType;

class RoseECGConstructSetGenerator : public ROSE_VisitorPatternDefaultBase {
public:
	RoseECGConstructSetGenerator() : nodeType(ECGNodeType::DEFAULT), csci(&cs) {}

	// TODO RN: memory leak anyone?
	ExtendedCallGraphNode* getECGNode() {
		return new ExtendedCallGraphNode(cs, nodeType);
	}

	InstRO::Core::ConstructSet getConstructSet() { return std::move(cs); }
	enum ECGNodeType getNodeType() { return nodeType; }

	void visit(SgFunctionDefinition* node) {
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
		nodeType = ECGNodeType::FUNCTION;
	}
	void visit(SgFunctionDeclaration* node) {
		assert(node);

		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getFragment(node, node->get_startOfConstruct()));
		nodeType = ECGNodeType::FUNCTION_CALL;
	}
	void visit(SgFunctionCallExp* node) {
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
		nodeType = ECGNodeType::FUNCTION_CALL;
	}
	void visit(SgIfStmt* node) {
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node->get_conditional()));
		nodeType = ECGNodeType::CONDITIONAL;
	}
	void visit(SgSwitchStatement* node) {
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node->get_item_selector()));
		nodeType = ECGNodeType::CONDITIONAL;
	}
	void visit(SgForStatement* node) {
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node->get_for_init_stmt()));
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node->get_test()));
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node->get_increment()));
		nodeType = ECGNodeType::LOOP;
	}
	void visit(SgWhileStmt* node) {
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node->get_condition()));
		nodeType = ECGNodeType::LOOP;
	}
	void visit(SgDoWhileStmt* node) {
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node->get_condition()));
		nodeType = ECGNodeType::LOOP;
	}
	void visit(SgBasicBlock* node) {
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getFragment(node, node->get_startOfConstruct()));
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getFragment(node, node->get_endOfConstruct()));
		nodeType = ECGNodeType::SCOPE;
	}

	void visit(SgNode* n) {
		// XXX this should not happen
		std::cout << "RoseECGConstructSetGenerator Error. Got invalid node" << std::endl
				<< n->unparseToString() << std::endl;
		exit(1);
	}

private:
	InstRO::Core::ConstructSet cs;
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface csci;

	enum ECGNodeType nodeType;
};

class RoseECGGenerator : public AstPrePostProcessing {
public:
	RoseECGGenerator() :
			callgraph(new ExtendedCallGraph()) {
	}
	~RoseECGGenerator() {}

	ExtendedCallGraph* build(SgProject* project) {

		std::vector<SgFunctionDefinition*> funcDefs = SageInterface::querySubTree<SgFunctionDefinition>(project, V_SgFunctionDefinition);
		for (SgFunctionDefinition* funcDef : funcDefs) {

			// visit function definition
			traverse(funcDef);
		}

		///XXX
		print(callgraph, "extendedCallGraph.dot");

		return callgraph;
	}

public:	// Visitor Interface
	void preOrderVisit(SgFunctionDefinition* node) {

		// XXX: we have to use DEFINITIONS here
		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);
		auto ecgNode = genCS.getECGNode();
		callgraph->addSgNode(ecgNode);

		currentVisitNode.push(ecgNode);
	}
	void preOrderVisit(SgFunctionCallExp* node) {
		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);

		// enclosing node -> callExp
		auto ecgNode = genCS.getECGNode();
		callgraph->addEdge(currentVisitNode.top(), ecgNode);

		// don't push current node
		// funcCalls can't have children expect their corresponding funcDecl

		SgFunctionDeclaration* calledDecl = node->getAssociatedFunctionDeclaration();
		if (calledDecl == nullptr) {
			return;	// FIXME MZ: calledDecl is not always available
		}

		SgFunctionDefinition* calledDef = tryGetDefiningDeclaration(calledDecl)->get_definition();
		if (calledDef == nullptr) {
			// no definition -> generate fragment
			RoseECGConstructSetGenerator genCSCalled;
			tryGetDefiningDeclaration(calledDecl)->accept(genCSCalled);
			callgraph->addEdge(ecgNode, genCSCalled.getECGNode());
		} else {
			RoseECGConstructSetGenerator genCSCalled;
			calledDef->accept(genCSCalled);
			callgraph->addEdge(ecgNode, genCSCalled.getECGNode());
		}
	}
	void preOrderVisit(SgIfStmt* node) {
		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);

		callgraph->addEdge(currentVisitNode.top(), genCS.getECGNode());
		currentVisitNode.push(callgraph->getGraphNode(genCS.getConstructSet()));
	}
	void preOrderVisit(SgSwitchStatement* node) {
		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);

		callgraph->addEdge(currentVisitNode.top(), genCS.getECGNode());
		currentVisitNode.push(callgraph->getGraphNode(genCS.getConstructSet()));
	}
	void preOrderVisit(SgForStatement* node) {
		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);

		callgraph->addEdge(currentVisitNode.top(), genCS.getECGNode());
		currentVisitNode.push(callgraph->getGraphNode(genCS.getConstructSet()));
	}
	void preOrderVisit(SgWhileStmt* node) {
		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);

		callgraph->addEdge(currentVisitNode.top(), genCS.getECGNode());
		currentVisitNode.push(callgraph->getGraphNode(genCS.getConstructSet()));
	}
	void preOrderVisit(SgDoWhileStmt* node) {
		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);

		callgraph->addEdge(currentVisitNode.top(), genCS.getECGNode());
		currentVisitNode.push(callgraph->getGraphNode(genCS.getConstructSet()));
	}
	void preOrderVisit(SgBasicBlock* node) {
		// skip fileScope
		if (!InstRO::Rose::Core::RoseConstructLevelPredicates::CLScopeStatementPredicate()(node)) {
			return;
		}

		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);

		assert(currentVisitNode.top());
		callgraph->addEdge(currentVisitNode.top(), genCS.getECGNode());
		currentVisitNode.push(callgraph->getGraphNode(genCS.getConstructSet()));
	}

	void postOrderVisit(SgFunctionDefinition* node) {
		currentVisitNode.pop();
	}
	void postOrderVisit(SgFunctionCallExp* node) {
		// nothing pushed -> nothing to pop
	}
	void postOrderVisit(SgIfStmt* node) {
		currentVisitNode.pop();
	}
	void postOrderVisit(SgSwitchStatement* node) {
		currentVisitNode.pop();
	}
	void postOrderVisit(SgForStatement* node) {
		currentVisitNode.pop();
	}
	void postOrderVisit(SgWhileStmt* node) {
		currentVisitNode.pop();
	}
	void postOrderVisit(SgDoWhileStmt* node) {
		currentVisitNode.pop();
	}
	void postOrderVisit(SgBasicBlock* node) {
		// skip fileScope
		if (!InstRO::Rose::Core::RoseConstructLevelPredicates::CLScopeStatementPredicate()(node)) {
			return;
		}
		currentVisitNode.pop();
	}

	// do manual dispatch here
	void preOrderVisit(SgNode* node) {
		// manual dispatch -_-
		if (isSgFunctionDefinition(node)) {
			preOrderVisit(isSgFunctionDefinition(node));
		} else if (isSgFunctionCallExp(node)) {
			preOrderVisit(isSgFunctionCallExp(node));
		} else if (isSgIfStmt(node)) {
			preOrderVisit(isSgIfStmt(node));
		} else if (isSgSwitchStatement(node)) {
			preOrderVisit(isSgSwitchStatement(node));
		} else if (isSgForStatement(node)) {
			preOrderVisit(isSgForStatement(node));
		} else if (isSgWhileStmt(node)) {
			preOrderVisit(isSgWhileStmt(node));
		} else if (isSgDoWhileStmt(node)) {
			preOrderVisit(isSgDoWhileStmt(node));
		} else if (isSgBasicBlock(node)) {
			preOrderVisit(isSgBasicBlock(node));
		}
	}
	void postOrderVisit(SgNode* node) {
		// manual dispatch -_-
		if (isSgFunctionDefinition(node)) {
			postOrderVisit(isSgFunctionDefinition(node));
		} else if (isSgFunctionCallExp(node)) {
			postOrderVisit(isSgFunctionCallExp(node));
		} else if (isSgIfStmt(node)) {
			postOrderVisit(isSgIfStmt(node));
		} else if (isSgSwitchStatement(node)) {
			postOrderVisit(isSgSwitchStatement(node));
		} else if (isSgForStatement(node)) {
			postOrderVisit(isSgForStatement(node));
		} else if (isSgWhileStmt(node)) {
			postOrderVisit(isSgWhileStmt(node));
		} else if (isSgDoWhileStmt(node)) {
			postOrderVisit(isSgDoWhileStmt(node));
		} else if (isSgBasicBlock(node)) {
			postOrderVisit(isSgBasicBlock(node));
		}
	}

private:
	ExtendedCallGraph* callgraph;

	std::stack<SgNode*> currentVisit;	// currently visited path
	std::stack<ExtendedCallGraphNode*>  currentVisitNode;
	std::map<std::string, SgFunctionDeclaration*> uniqueDecls;	// unique function-declaration per function

private:
	InstRO::Core::ConstructSet generateConstructSet(SgNode* node) {
		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);
		return genCS.getConstructSet();
	}

	SgFunctionDeclaration* getDefiningDeclaration(SgFunctionDeclaration* oldDecl) {
		SgDeclarationStatement* definingDecl = oldDecl->get_definingDeclaration();
		return isSgFunctionDeclaration(definingDecl);
	}


	SgFunctionDeclaration* tryGetDefiningDeclaration(SgFunctionDeclaration* oldDecl) {
		SgFunctionDeclaration* definingDecl = getDefiningDeclaration(oldDecl);
		if (definingDecl) {
			return definingDecl;
		}
		return oldDecl;
	}

	void print(ExtendedCallGraph* callgraph, std::string filename) {

		std::ofstream outfile(filename, std::ofstream::out);
		outfile << "digraph callgraph {\nnode [shape=oval]\n";

		for (ExtendedCallGraphNode* fromNode : callgraph->getNodeSet()) {

			outfile << dumpToDotString(callgraph, fromNode) << std::endl;

			for (ExtendedCallGraphNode* toNode : callgraph->getSuccessors(fromNode)) {
				outfile << "\"" << fromNode << "\" -> \"" << toNode << "\"" << std::endl;
			}

		}
		outfile << "\n}" << std::endl;
		outfile.close();
	}

	std::string dumpToDotString(ExtendedCallGraph* callgraph, ExtendedCallGraphNode* node) {

		std::string nodeType;
		std::string color = "black";


		switch (node->getNodeType()) {
		case ECGNodeType::FUNCTION:
			nodeType = "FUNCTION";
			color = "red";
			break;
		case ECGNodeType::FUNCTION_CALL:
			nodeType = "CALL";
			color = "green";
			break;
		case ECGNodeType::CONDITIONAL:
			nodeType = "CONDITIONAL";
			color = "blue";
			break;
		case ECGNodeType::LOOP:
			nodeType = "LOOP";
			color = "orange";
			break;
		case ECGNodeType::SCOPE:
			nodeType = "SCOPE";
			break;
		case ECGNodeType::DEFAULT:
			nodeType = "DEFAULT";
			break;
		default:
			assert(false);
		}

		std::stringstream ss;
		ss <<  "\"" << node << "\" [label=\"" << nodeType << "\\n" << *node
				<< "\", color=" << color << "]";

		return ss.str();
	}

//	SgFunctionDeclaration* getUniqueDeclaration(SgFunctionDeclaration* funcDecl) {
//		funcDecl = tryGetDefiningDeclaration(funcDecl);
//		std::string mangledName = funcDecl->get_mangled_name();
//
//		if (uniqueDecls.find(mangledName) != uniqueDecls.end()) {
//			if (funcDecl->get_definition()) {
//
//				callgraph->swapSgNode(uniqueDecls[mangledName], funcDecl);
//				uniqueDecls[mangledName] = funcDecl;
//			}
//			return uniqueDecls[mangledName];
//		} else {
//			uniqueDecls[mangledName] = funcDecl;
//			return funcDecl;
//		}
//	}
};

}
}
}
}
