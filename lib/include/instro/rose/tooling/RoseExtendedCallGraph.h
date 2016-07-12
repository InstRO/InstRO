#ifndef INSTRO_ROSE_TOOLING_ROSE_EXTENDED_CALL_GRAPH_H
#define INSTRO_ROSE_TOOLING_ROSE_EXTENDED_CALL_GRAPH_H

#include "rose.h"

#include "instro/rose/core/RoseConstructSet.h"
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
	RoseECGConstructSetGenerator() : nodeType(ECGNodeType::DEFAULT), csci(&cs) {}

	// XXX this is smelly
	ExtendedCallGraphNode* getECGNode() { return new ExtendedCallGraphNode(cs, nodeType); }

	InstRO::Core::ConstructSet getConstructSet() { return std::move(cs); }
	enum ECGNodeType getNodeType() { return nodeType; }

	void visit(SgFunctionDefinition* node);

	void visit(SgFunctionDeclaration* node);

	void visit(SgFunctionCallExp* node);
	void visit(SgIfStmt* node);
	void visit(SgSwitchStatement* node);
	void visit(SgForStatement* node);
	void visit(SgWhileStmt* node);
	void visit(SgDoWhileStmt* node);
	void visit(SgBasicBlock* node);

	void visit(SgNode* n);	// Should not happen and aborts the program.

 private:
	void addConditionStmtOrExpr(SgStatement* conditionStmt);

	InstRO::Core::ConstructSet cs;
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface csci;

	enum ECGNodeType nodeType;
};

class RoseECGGenerator : public AstPrePostProcessing {
 public:
	RoseECGGenerator() : callgraph(new ExtendedCallGraph()) {}
	~RoseECGGenerator() {}

	ExtendedCallGraph* build(SgProject* project);

 private:
	ExtendedCallGraphNode* getDefiningECGNode(SgFunctionDefinition* node);

	/** this method is only called if the definition cannot be found */
	ExtendedCallGraphNode* getDefiningECGNode(SgFunctionDeclaration* node);

 public:	// Visitor Interface
	void preOrderVisit(SgFunctionDefinition* node);

	void preOrderVisit(SgFunctionCallExp* node);

	void preOrderVisit(SgIfStmt* node) { defaultPreOrderBehavior(node); }
	void preOrderVisit(SgSwitchStatement* node) { defaultPreOrderBehavior(node); }
	void preOrderVisit(SgForStatement* node) { defaultPreOrderBehavior(node); }
	void preOrderVisit(SgWhileStmt* node) { defaultPreOrderBehavior(node); }
	void preOrderVisit(SgDoWhileStmt* node) { defaultPreOrderBehavior(node); }
	void preOrderVisit(SgBasicBlock* node);
	void postOrderVisit(SgFunctionDefinition* node) { currentVisitNode.pop(); }
	void postOrderVisit(SgFunctionCallExp* node) {
		// nothing pushed -> nothing to pop
	}
	void postOrderVisit(SgIfStmt* node) { currentVisitNode.pop(); }
	void postOrderVisit(SgSwitchStatement* node) { currentVisitNode.pop(); }
	void postOrderVisit(SgForStatement* node) { currentVisitNode.pop(); }
	void postOrderVisit(SgWhileStmt* node) { currentVisitNode.pop(); }
	void postOrderVisit(SgDoWhileStmt* node) { currentVisitNode.pop(); }
	void postOrderVisit(SgBasicBlock* node);
	// do manual dispatch here
	void preOrderVisit(SgNode* node);
	void postOrderVisit(SgNode* node);

 private:
	ExtendedCallGraph* callgraph;

	std::stack<ExtendedCallGraphNode*> currentVisitNode;

	std::map<InstRO::Core::ConstructSet, ExtendedCallGraphNode*> csToGraphNode;

	std::map<std::string, SgFunctionDeclaration*> uniqueDecls;	// unique function-declaration per function
	std::map<std::string, ExtendedCallGraphNode*> uniqueNodes;	// unique graph node per mangled function name

 private:
	void defaultPreOrderBehavior(SgNode* node);

	SgFunctionDeclaration* getDefiningDeclaration(SgFunctionDeclaration* oldDecl);

	SgFunctionDeclaration* tryGetDefiningDeclaration(SgFunctionDeclaration* oldDecl);
};
}
}
}
}

#endif
