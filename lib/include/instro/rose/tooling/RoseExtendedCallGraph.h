#include "rose.h"

#include "instro/tooling/ExtendedCallGraph.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace ExtendedCallGraph {

using InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph;
using InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode;
using InstRO::Tooling::ExtendedCallGraph::ECGNodeType;

class RoseECGConstructSetGenerator : public ROSE_VisitorPatternDefaultBase {
public:
	RoseECGConstructSetGenerator() : nodeType(ECGNodeType::DEFAULT) {}

	ExtendedCallGraphNode* getECGNode() {
		return new ExtendedCallGraphNode(cs, nodeType);
	}

	InstRO::Core::ConstructSet getConstructSet() { return std::move(cs); }
	enum ECGNodeType getNodeType() { return nodeType; }

	void visit(SgNode* n) {
		// TODO
		std::cout << "error" << std::endl;
		exit(1);
	}

private:
	InstRO::Core::ConstructSet cs;
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

		return callgraph;
	}

public:	// Visitor Interface
	void preOrderVisit(SgFunctionDefinition* node) {
		// XXX: we have to use DEFINITIONS here
		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);
		currentVisitNode.push(genCS.getECGNode());
	}
	void preOrderVisit(SgFunctionCallExp* node) {
		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);

		// enclosing node -> callExp
		callgraph->addEdge(currentVisitNode.top(), genCS.getECGNode());

		// don't push current node
		// funcCalls can't have children expect their corresponding funcDecl

		SgFunctionDeclaration* calledDecl = node->getAssociatedFunctionDeclaration();
		if (calledDecl == nullptr) {
			return;	// FIXME MZ: calledDecl is not always available
		}

//		calledDecl = getUniqueDeclaration(tryGetDefiningDeclaration(calledDecl));
//		// callExp -> call funcDecl
//		// TODO handle this case!
//		callgraph->addSgEdge(node, calledDecl);
	}
	void preOrderVisit(SgIfStmt* node) {
		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);

		callgraph->addEdge(currentVisitNode.top(), genCS.getECGNode());
		currentVisitNode.push(callgraph->getGraphNode(genCS.getConstructSet()));
	}
	void preOrderVisit(SgSwitchStatement* node) {
//		callgraph->addSgEdge(currentVisit.top(), node);
//		currentVisit.push(node);
	}
	void preOrderVisit(SgForStatement* node) {
//		callgraph->addSgEdge(currentVisit.top(), node);
//		currentVisit.push(node);
	}
	void preOrderVisit(SgWhileStmt* node) {
//		callgraph->addSgEdge(currentVisit.top(), node);
//		currentVisit.push(node);
	}
	void preOrderVisit(SgDoWhileStmt* node) {
//		callgraph->addSgEdge(currentVisit.top(), node);
//		currentVisit.push(node);
	}
	void preOrderVisit(SgBasicBlock* node) {
//		callgraph->addSgEdge(currentVisit.top(), node);
//		currentVisit.push(node);
	}

	void postOrderVisit(SgFunctionDefinition* node) {
		currentVisit.pop();
	}
	void postOrderVisit(SgFunctionCallExp* node) {
		// nothing pushed -> nothing to pop
	}
	void postOrderVisit(SgIfStmt* node) {
		currentVisit.pop();
	}
	void postOrderVisit(SgSwitchStatement* node) {
		currentVisit.pop();
	}
	void postOrderVisit(SgForStatement* node) {
		currentVisit.pop();
	}
	void postOrderVisit(SgWhileStmt* node) {
		currentVisit.pop();
	}
	void postOrderVisit(SgDoWhileStmt* node) {
		currentVisit.pop();
	}
	void postOrderVisit(SgBasicBlock* node) {
		currentVisit.pop();
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
