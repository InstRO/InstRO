#ifndef LIB_INCLUDE_INSTRO_V5_PREVIEW_CONSTRUCTSET_H_
#define LIB_INCLUDE_INSTRO_V5_PREVIEW_CONSTRUCTSET_H_
/* Base Implementation of the Construct for ROSE.
	 The RoseConstructProvider produces constructs based on the AST.
*/

#include "instro/core/ConstructSet.h"
#include <map>

#include "rose.h"

namespace InstRO {
namespace Rose {
namespace Core {

namespace RoseConstructLevelPredicates {
struct CLExpressionPredicate {
	bool operator()(SgNode* n) const { return isSgExpression(n) != nullptr; }
};

struct CLLoopPredicate {
	bool operator()(SgNode* n) const {
		if (isSgDoWhileStmt(n) || isSgWhileStmt(n) || isSgForStatement(n)) {
			return true;
		}
		return false;
	}
};

struct CLConditionalPredicate {
	bool operator()(SgNode* n) const {
		if (isSgIfStmt(n) || isSgSwitchStatement(n)) {
			return true;
		}
		return false;
	}
};

struct CLScopeStatementPredicate {
	bool operator()(SgNode* n) const {
		// ignore function scopes
		if (isSgBasicBlock(n) && !isSgFunctionDefinition(n->get_parent())) {
			return true;
		}
		return false;
	}
};

struct CLStatementPredicate {
	bool operator()(SgNode* n) const {
		if (isSgDeclarationStatement(n)) {
			if (isSgVariableDeclaration(n) && (isSgVariableDeclaration(n)->get_definition() != nullptr)) {
				return true;	// only variable declarations with initializer
			}
			return false;
		}

		if (isSgScopeStatement(n)) {
			if (CLScopeStatementPredicate()(n)) {
				return true;	// only scope statements
			}
			return false;
		}

		if (isSgStatement(n)) {
			return true;
		}
		return false;
	}
};

struct CLFunctionPredicate {
	bool operator()(SgNode* n) const { return isSgFunctionDefinition(n) != nullptr; }
};

struct CLFileScopePredicate {
	bool operator()(SgNode* n) const { return isSgFile(n) != nullptr; }
};

struct CLGlobalScopePredicate {
	bool operator()(SgNode* n) const { return isSgProject(n) != nullptr; }
};

struct CLSimpleStatementPredicate {
	bool operator()(SgNode* n) const {
		if (CLStatementPredicate()(n)) {
			if (CLScopeStatementPredicate()(n) || CLConditionalPredicate()(n) || CLLoopPredicate()(n)) {
				return false;
			} else {
				return true;
			}
		}
		return false;
	}
};

struct WrappableConstructPredicate {
	bool operator()(SgNode* n) const {
		return isSgBasicBlock(n->get_parent());
	}
};

struct InstrumentableConstructPredicate {
	// TODO: how exactly is this defined?
	bool operator()(SgNode* n) const;
};

struct ConstructPredicate {
	bool operator()(SgNode* n) const {
		return CLGlobalScopePredicate()(n) || CLFileScopePredicate()(n) || CLFunctionPredicate()(n)
				|| CLStatementPredicate()(n) || CLExpressionPredicate()(n);
	}
};
}	// namespace RoseConstructLevelPredicates

class ConstructGenerator : public ROSE_VisitorPatternDefaultBase {
 public:
	ConstructGenerator() : cl(InstRO::Core::ConstructLevelType::CLNotALevel) {};
	InstRO::Core::ConstructLevelType getCLT() { return cl; }

	// global scope
	void visit(SgProject* node) {
		cl = InstRO::Core::ConstructLevelType::CLGlobalScope;
	}

	// file scope
	void visit(SgSourceFile* node) {
		cl = InstRO::Core::ConstructLevelType::CLFileScope;
	}

	// function
	void visit(SgFunctionDefinition* node) { cl = InstRO::Core::ConstructLevelType::CLFunction; }

	// conditionals
	void visit(SgIfStmt* node) { cl = InstRO::Core::ConstructLevelType::CLConditionalStatement; }
	void visit(SgSwitchStatement* node) { cl = InstRO::Core::ConstructLevelType::CLConditionalStatement; }

	// loops
	void visit(SgForStatement* node) { cl = InstRO::Core::ConstructLevelType::CLLoopStatement; }
	void visit(SgWhileStmt* node) { cl = InstRO::Core::ConstructLevelType::CLLoopStatement; }
	void visit(SgDoWhileStmt* node) { cl = InstRO::Core::ConstructLevelType::CLLoopStatement; }

	// scopes
	void visit(SgBasicBlock* node) {
		if (RoseConstructLevelPredicates::CLConditionalPredicate()(node)) {
			cl = InstRO::Core::ConstructLevelType::CLScopeStatement;
		} else {
			generateError(node);
		}
	}

	// statements
	void visit(SgStatement* node) {
		if (RoseConstructLevelPredicates::CLSimpleStatementPredicate()(node)) {
			cl = InstRO::Core::ConstructLevelType::CLSimpleStatement;
		} else {
			generateError(node);
		}
	}

	// expressions
	void visit(SgExpression* node) { cl = InstRO::Core::ConstructLevelType::CLExpression; }
	void visit(SgVariableDeclaration* node) {
		// CI: an initialized variable declaration is OK,
		if (node->get_definition()) {
			cl = InstRO::Core::ConstructLevelType::CLSimpleStatement;
		} else {
			generateError(node);
		}
	}

	// this should be an error
	void visit(SgScopeStatement* node) { generateError(node); }
	void visit(SgDeclarationStatement* node) { generateError(node); }
	void visit(SgNode* node) { generateError(node); }

 private:
	InstRO::Core::ConstructLevelType cl;

	void generateError(SgNode* node) {
		std::cout << "# Encountered error case in ConstructGenerator. " << node->class_name() << "\t"
							<< node->unparseToString() << std::endl;
	}
};

class RoseConstruct : public InstRO::Core::Construct {
 public:
	RoseConstruct(SgNode* sgnode) : Construct(InstRO::Core::ConstructLevelType::CLNotALevel), node(sgnode) {
		if (sgnode != nullptr) {
			ConstructGenerator gen;
			node->accept(gen);
			construct_level = gen.getCLT();
		}
	}

	virtual ~RoseConstruct() {}

	::SgNode* getNode() const { return node; }

 private:
	::SgNode* node;
};

class RoseConstructProvider {
 public:
	static RoseConstructProvider& getInstance() {
		static RoseConstructProvider instance;
		return instance;
	}

	std::shared_ptr<RoseConstruct> getConstruct(SgNode* node) {
		std::cout << "getConstruct(" << node << ")" << std::endl;
		if (mapping.find(node) != mapping.end()) {
			std::cout << "\tfound existing Construct" << std::endl;
			return mapping[node];
		}
		std::cout << "\tcreating new construct" << std::endl;
		std::shared_ptr<RoseConstruct> construct = std::shared_ptr<RoseConstruct>(new RoseConstruct(node));
		mapping[node] = construct;
		return construct;
	}

 private:
	std::map<SgNode*, std::shared_ptr<RoseConstruct> > mapping;

 private:
	RoseConstructProvider(){};
	RoseConstructProvider(RoseConstructProvider&) = delete;
	void operator=(RoseConstructProvider const&) = delete;
};
}
}
}	// Namespace InstRO

#endif /* LIB_INCLUDE_INSTRO_V5_PREVIEW_CONSTRUCTSET_H_ */
