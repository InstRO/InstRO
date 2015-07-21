#ifndef LIB_INCLUDE_INSTRO_V5_PREVIEW_CONSTRUCTSET_H_
#define LIB_INCLUDE_INSTRO_V5_PREVIEW_CONSTRUCTSET_H_
/* Base Implementation of the Construct for ROSE.
	 The RoseConstructProvider produces constructs based on the AST.
*/


#include <iostream>
#include <map>

#include "rose.h"

#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Rose {
namespace Core {

namespace RoseConstructLevelPredicates {
struct CTPredicate {
	virtual bool operator()(SgNode* n) const {
		// RN: TODO will this actually work with a base implementation?
		return false;
	}
	virtual ~CTPredicate() {}
};

struct CLExpressionPredicate : public CTPredicate {
	bool operator()(SgNode* n) const {
		return isSgExpression(n) != nullptr;
	}
};

struct CLLoopPredicate : public CTPredicate {
	bool operator()(SgNode* n) const {
		if (isSgDoWhileStmt(n) || isSgWhileStmt(n) || isSgForStatement(n)) {
			return true;
		}
		return false;
	}
};

struct CLConditionalPredicate : public CTPredicate {
	bool operator()(SgNode* n) const {
		if (isSgIfStmt(n) || isSgSwitchStatement(n)) {
			return true;
		}
		return false;
	}
};

struct CLScopeStatementPredicate : public CTPredicate {
	bool operator()(SgNode* n) const {
		// ignore function scopes
		if (isSgBasicBlock(n) && !isSgFunctionDefinition(n->get_parent())) {
			return true;
		}
		return false;
	}
};

struct CLStatementPredicate : public CTPredicate {
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

struct CLFunctionPredicate : public CTPredicate {
	bool operator()(SgNode* n) const { return isSgFunctionDefinition(n) != nullptr; }
};

struct CLFileScopePredicate : public CTPredicate {
	bool operator()(SgNode* n) const { return isSgFile(n) != nullptr; }
};

struct CLGlobalScopePredicate : public CTPredicate {
	bool operator()(SgNode* n) const { return isSgProject(n) != nullptr; }
};

struct CLSimpleStatementPredicate : public CTPredicate {
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

struct CTWrappableStatementPredicate : public CTPredicate {
	bool operator()(SgNode* n) const {
		return (isSgBasicBlock(n->get_parent())!=nullptr);
	}
};

struct InstrumentableConstructPredicate : public CTPredicate {
	// TODO: how exactly is this defined?
	bool operator()(SgNode* n) const;
};

struct ConstructPredicate : public CTPredicate {
	bool operator()(SgNode* n) const {
		return CLGlobalScopePredicate()(n) || CLFileScopePredicate()(n) || CLFunctionPredicate()(n)
				|| CLStatementPredicate()(n) || CLExpressionPredicate()(n);
	}
};

//// TODO actually use that mechanism
CTPredicate getPredicateForTraitType(InstRO::Core::ConstructTraitType traitType);

}	// namespace RoseConstructLevelPredicates

class ConstructGenerator : public ROSE_VisitorPatternDefaultBase {
 public:
	ConstructGenerator() : ct(InstRO::Core::ConstructTraitType::CTNoTraits) {};
	InstRO::Core::ConstructTrait getConstructTraits() { return ct; }

	// global scope
	void visit(SgProject* node) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTGlobalScope);
	}

	// file scope
	void visit(SgSourceFile* node) {
		ct.add(InstRO::Core::ConstructTraitType::CTFileScope);
	}

	// function
	void visit(SgFunctionDefinition* node) {
		ct.add(InstRO::Core::ConstructTraitType::CTFunction);
	}

	// conditionals
	void visit(SgIfStmt* node) {
		ct.add(InstRO::Core::ConstructTraitType::CTConditionalStatement);
		handleWrappableCheck(node);
	}
	void visit(SgSwitchStatement* node) {
		ct = InstRO::Core::ConstructTraitType::CTConditionalStatement;
		handleWrappableCheck(node);
	}

	// loops
	void visit(SgForStatement* node) {
		ct = InstRO::Core::ConstructTraitType::CTLoopStatement;
		handleWrappableCheck(node);
	}
	void visit(SgWhileStmt* node) {
		ct = InstRO::Core::ConstructTraitType::CTLoopStatement;
		handleWrappableCheck(node);
	}
	void visit(SgDoWhileStmt* node) {
		ct = InstRO::Core::ConstructTraitType::CTLoopStatement;
		handleWrappableCheck(node);
	}

	// scopes
	void visit(SgBasicBlock* node) {
		if (RoseConstructLevelPredicates::CLConditionalPredicate()(node)) {
			ct = InstRO::Core::ConstructTraitType::CTScopeStatement;
			handleWrappableCheck(node);
		} else {
			generateError(node);
		}
	}

	// statements
	void visit(SgStatement* node) {
		if (RoseConstructLevelPredicates::CLSimpleStatementPredicate()(node)) {
			ct = InstRO::Core::ConstructTraitType::CTSimpleStatement;
			handleWrappableCheck(node);
		} else {
			generateError(node);
		}
	}

	// expressions
	void visit(SgExpression* node) { ct = InstRO::Core::ConstructTraitType::CTExpression; }
	void visit(SgVariableDeclaration* node) {
		// CI: an initialized variable declaration is OK,
		if (node->get_definition()) {
			ct = InstRO::Core::ConstructTraitType::CTSimpleStatement;
			handleWrappableCheck(node);
		} else {
			generateError(node);
		}
	}

	// this should be an error
	void visit(SgScopeStatement* node) { generateError(node); }
	void visit(SgDeclarationStatement* node) { generateError(node); }
	void visit(SgNode* node) { generateError(node); }

 private:
	InstRO::Core::ConstructTrait ct;

	void handleWrappableCheck(SgNode* node) {
		ct.add(InstRO::Core::ConstructTraitType::CTStatement);
		if (RoseConstructLevelPredicates::CTWrappableStatementPredicate()(node)) {
			ct.add(InstRO::Core::ConstructTraitType::CTWrappableStatement);
		}
	}

	void generateError(SgNode* node) {
		std::cout << "# Encountered error case in ConstructGenerator. " << node->class_name() << "\t"
							<< node->unparseToString() << std::endl;
	}
};



class RoseConstruct : public InstRO::Core::Construct {
 public:
	size_t getID(){return (size_t)node;}
	RoseConstruct(SgNode* sgnode, InstRO::Core::ConstructTrait traits) :
			InstRO::Core::Construct(traits), node(sgnode) {
	}
	virtual ~RoseConstruct() {}

	SgNode* getNode() const { return node; }

	virtual std::string toString() override {
		return "RoseConstruct: " + node->class_name() + ": " + node->unparseToString();
	}

 private:
	SgNode* node;
};

class RoseFragment : public RoseConstruct {
public:
	RoseFragment(SgNode * associatedNode, Sg_File_Info* info) :
		RoseConstruct(associatedNode,InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTFragment)), info(info) {
	}
	size_t getID(){return (size_t)info;};
	~RoseFragment() {}

	Sg_File_Info* getFileInfo() {
		return info;
	}

	std::string toString() override {
		std::stringstream ss;
		ss << "RoseFragment line:" << info->get_line() << " col:" << info->get_col();
		return ss.str();
	}

private:
	Sg_File_Info* info;
};

class RoseConstructProvider {
 public:
	static RoseConstructProvider& getInstance() {
		static RoseConstructProvider instance;
		return instance;
	}

	std::shared_ptr<RoseConstruct> getConstruct(SgNode* node) {

		std::cout << "getConstruct(" << node << ")" << std::endl;
		if (node == nullptr) {
			throw std::string("RoseConstructProvider: attempted to getConstruct for nullptr");
		}

		if (mapping.find(node) == mapping.end()) {
			std::cout << "\tcreating new construct" << std::endl;

			ConstructGenerator gen;
			node->accept(gen);
			mapping[node] = std::shared_ptr<RoseConstruct>(new RoseConstruct(node, gen.getConstructTraits()));
		}
		return mapping[node];
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
