#include "rose.h"

#include "instro/utility/Logger.h"

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

		if (node->get_definition() != nullptr) {
			visit(node->get_definition());
			return;
		}

		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getFragment(node, node->get_startOfConstruct()));
		nodeType = ECGNodeType::FUNCTION;
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
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface csci;

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
		callgraph->print("extendedCallGraph.dot");

		return callgraph;
	}

private:
	ExtendedCallGraphNode* getDefiningECGNode(SgFunctionDefinition* node) {
		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);
		auto ecgNode = genCS.getECGNode();

		std::string mangledName = node->get_declaration()->get_mangled_name();
		if (uniqueDecls.find(mangledName) != uniqueDecls.end()) {

			if (uniqueDecls[mangledName]->get_definition() == nullptr) {
				callgraph->swapConstructSet(uniqueNodes[mangledName]->getAssociatedConstructSet(), ecgNode->getAssociatedConstructSet());
			} else {
				delete ecgNode;
			}

		} else {
			// add node so it is not missing, if it has no children
			callgraph->addNode(ecgNode);
			uniqueDecls[mangledName] = node->get_declaration();
			uniqueNodes[mangledName] = ecgNode;
		}

		return uniqueNodes[mangledName];

	}

	/** this method is only called if the definition cannot be found */
	ExtendedCallGraphNode* getDefiningECGNode(SgFunctionDeclaration* node) {
		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);
		auto ecgNode = genCS.getECGNode();

		std::string mangledName = node->get_mangled_name();
		if (uniqueDecls.find(mangledName) == uniqueDecls.end()) {
			callgraph->addNode(ecgNode);
			uniqueDecls[mangledName] = node;
			uniqueNodes[mangledName] = ecgNode;
		} else {
			delete ecgNode;
		}
		return uniqueNodes[mangledName];
	}

public:	// Visitor Interface
	void preOrderVisit(SgFunctionDefinition* node) {

		auto ecgNode = getDefiningECGNode(node);

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

			logIt(WARN) << "Virtual call in line "<< node->get_startOfConstruct()->get_line() << ": " << node->unparseToString() << std::endl;

			auto arrowExp = isSgArrowExp(node->get_function());
			if (arrowExp == nullptr) {
				return;
			}

			auto memberFuncRefExp = isSgMemberFunctionRefExp(arrowExp->get_rhs_operand());
			if (memberFuncRefExp == nullptr) {
				return;
			}

			calledDecl = memberFuncRefExp->getAssociatedMemberFunctionDeclaration();
			if (calledDecl == nullptr) {
				logIt(WARN) << "No associated function declaration for: " << node->unparseToString() << std::endl;
				return;	// FIXME MZ: calledDecl is not always available
			}
		}

		SgFunctionDefinition* calledDef = tryGetDefiningDeclaration(calledDecl)->get_definition();
		if (calledDef == nullptr) {
			auto ecgNodeCall = getDefiningECGNode(calledDecl);
			callgraph->addEdge(ecgNode, ecgNodeCall);
		} else {
			auto ecgNodeCall = getDefiningECGNode(calledDef);
			callgraph->addEdge(ecgNode, ecgNodeCall);
		}
	}
	void preOrderVisit(SgIfStmt* node) {
		defaultPreOrderBehavior(node);
	}
	void preOrderVisit(SgSwitchStatement* node) {
		defaultPreOrderBehavior(node);
	}
	void preOrderVisit(SgForStatement* node) {
		defaultPreOrderBehavior(node);
	}
	void preOrderVisit(SgWhileStmt* node) {
		defaultPreOrderBehavior(node);
	}
	void preOrderVisit(SgDoWhileStmt* node) {
		defaultPreOrderBehavior(node);
	}
	void preOrderVisit(SgBasicBlock* node) {
		// skip fileScope
		if (!InstRO::Rose::Core::RoseConstructLevelPredicates::CLScopeStatementPredicate()(node)) {
			return;
		}
		defaultPreOrderBehavior(node);
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
		if (!InstRO::Rose::Core::RoseConstructLevelPredicates::CLScopeStatementPredicate()(node)) {
			return;	// skip fileScope
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

	std::stack<ExtendedCallGraphNode*>  currentVisitNode;

	std::map<InstRO::Core::ConstructSet, ExtendedCallGraphNode*> csToGraphNode;

	std::map<std::string, SgFunctionDeclaration*> uniqueDecls;	// unique function-declaration per function
	std::map<std::string, ExtendedCallGraphNode*> uniqueNodes;	// unique graph node per mangled function name

private:
	void defaultPreOrderBehavior(SgNode* node) {
		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);

		callgraph->addEdge(currentVisitNode.top(), genCS.getECGNode());
		currentVisitNode.push(callgraph->getNodeWithExactConstructSet(genCS.getConstructSet()));
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

};

}
}
}
}
