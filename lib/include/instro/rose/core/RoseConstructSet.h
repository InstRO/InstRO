#ifndef LIB_INCLUDE_INSTRO_ROSE_CORE_CONSTRUCTSET_H_
#define LIB_INCLUDE_INSTRO_ROSE_CORE_CONSTRUCTSET_H_
/* Base Implementation of the Construct for ROSE.
	 The RoseConstructProvider produces constructs based on the AST.
*/

#include <iostream>
#include <map>
#include <boost/algorithm/string.hpp>

#include "rose.h"

#include "instro/core/ConstructSet.h"
#include "instro/utility/exception.h"
#include "instro/utility/Logger.h"

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

struct DefinedVariableDeclarationPredicate {
	///XXX that is so dumb, there has to be an easier way
	bool operator()(SgNode* n) const {
		auto initNames = isSgVariableDeclaration(n)->get_variables();
		return initNames[0]->get_initptr() != nullptr;
	}
};

struct CLExpressionPredicate : public CTPredicate {
	bool operator()(SgNode* n) const {
		if (isSgExprListExp(n) != nullptr)
			return false;
		if (isSgFunctionRefExp(n) != nullptr)
			return false;
		if (isSgFunctionCallExp(n) != nullptr)
			return true;
		// for variables and values, we only accept as instrumentable, if the expression itself has an observable effect,
		// e.g. as conditional in an if or for
		if (isSgValueExp(n) != nullptr || isSgIntVal(n) != nullptr || isSgStringVal(n) != nullptr || isSgVarRefExp(n) != nullptr) {
			// In Rose this is TRUE !!if!! the parent of the stmt is an SgExprStatement and the parent(parent) is either the for loops
			// conditional or the conditional of an if or while
			SgNode* parent = n->get_parent();
			if (parent == nullptr)
				return false;
			if (isSgExprStatement(parent) == nullptr)
				return false;
			SgNode* grandParent = parent->get_parent();
			if (grandParent == nullptr)
				return false;
			if (isSgIfStmt(grandParent) != nullptr && isSgIfStmt(grandParent)->get_conditional() == parent)
				return true;
			else if (isSgForStatement(grandParent) != nullptr && isSgForStatement(grandParent)->get_test() == parent)
				return true;
			else if (isSgDoWhileStmt(grandParent) && isSgDoWhileStmt(grandParent)->get_condition() == parent)
				return true;
			else if (isSgWhileStmt(grandParent) && isSgWhileStmt(grandParent)->get_condition() == parent)
				return true;
			else
				return false;
		}
		if (isSgCastExp(n) != nullptr)
			return false;
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
			if (isSgVariableDeclaration(n) && DefinedVariableDeclarationPredicate()(n)) {
				return true;	// only variable declarations with initializer
			}
			return false;
		}
		if (isSgIfStmt(n) || isSgSwitchStatement(n) || isSgDoWhileStmt(n) || isSgWhileStmt(n) || isSgForStatement(n))
			return true;

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
	bool operator()(SgNode* n) const { return (isSgBasicBlock(n->get_parent()) != nullptr); }
};

struct InstrumentableConstructPredicate : public CTPredicate {
	// TODO: how exactly is this defined?
	bool operator()(SgNode* n) const;
};

struct ConstructPredicate : public CTPredicate {
	bool operator()(SgNode* n) const {
		return CLGlobalScopePredicate()(n) || CLFileScopePredicate()(n) || CLFunctionPredicate()(n) ||
					 CLStatementPredicate()(n) || CLExpressionPredicate()(n);
	}
};


//// TODO actually use that mechanism
CTPredicate getPredicateForTraitType(InstRO::Core::ConstructTraitType traitType);

}	// namespace RoseConstructLevelPredicates

class ConstructGenerator : public ROSE_VisitorPatternDefaultBase {
 public:
	ConstructGenerator() : ct(InstRO::Core::ConstructTraitType::CTNoTraits){};
	InstRO::Core::ConstructTrait getConstructTraits() { return ct; }

	// global scope
	void visit(SgProject* node) { ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTGlobalScope); }

	// file scope
	void visit(SgSourceFile* node) { ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTFileScope); }

	// function
	void visit(SgFunctionDefinition* node) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTFunction);
	}

	// conditionals
	void visit(SgIfStmt* node) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTConditionalStatement);
		handleStatementWithWrappableCheck(node);
	}
	void visit(SgSwitchStatement* node) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTConditionalStatement);
		handleStatementWithWrappableCheck(node);
	}

	// loops
	void visit(SgForStatement* node) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTLoopStatement);
		handleStatementWithWrappableCheck(node);
	}
	void visit(SgWhileStmt* node) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTLoopStatement);
		handleStatementWithWrappableCheck(node);
	}
	void visit(SgDoWhileStmt* node) {
		ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTLoopStatement);
		handleStatementWithWrappableCheck(node);
	}

	// scopes
	void visit(SgBasicBlock* node) {
		if (RoseConstructLevelPredicates::CLScopeStatementPredicate()(node)) {
			ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTScopeStatement);
			handleStatementWithWrappableCheck(node);
		} else {
			generateError(node);
		}
	}

	// statements
	void visit(SgStatement* node) {
		if (RoseConstructLevelPredicates::CLSimpleStatementPredicate()(node)) {
			ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTSimpleStatement);
			handleStatementWithWrappableCheck(node);
		} else {
			generateError(node);
		}
	}

	void visit(SgVariableDeclaration* node) {
		// CI: an initialized variable declaration is OK,
		if (RoseConstructLevelPredicates::DefinedVariableDeclarationPredicate()(node)) {
			ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTSimpleStatement);
			handleStatementWithWrappableCheck(node);
		} else {
			generateError(node);
		}
	}

	// expressions
	void visit(SgExpression* node) { ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTExpression); }

	// this should be an error
	void visit(SgScopeStatement* node) { generateError(node); }
	void visit(SgDeclarationStatement* node) { generateError(node); }
	void visit(SgNode* node) { generateError(node); }

 private:
	InstRO::Core::ConstructTrait ct;

	void handleStatementWithWrappableCheck(SgNode* node) {
		ct.add(InstRO::Core::ConstructTraitType::CTStatement);
		if (RoseConstructLevelPredicates::CTWrappableStatementPredicate()(node)) {
			ct.add(InstRO::Core::ConstructTraitType::CTWrappableStatement);
		}
	}

	void generateError(SgNode* node) {
		logIt(ERROR) << "# Encountered error case in ConstructGenerator. " << node->class_name() << "\t"
							<< node->unparseToString() << std::endl;
	}
};

class RoseConstruct : public InstRO::Core::Construct {
 public:
	RoseConstruct(SgNode* sgnode, InstRO::Core::ConstructTrait traits) : InstRO::Core::Construct(traits), node(sgnode) {}
	virtual ~RoseConstruct() {}

	size_t getID() const { return (size_t)node; }
	SgNode* getNode() const { return node; }

	// we construct the identification as follows:
	// filename:startline--ConstructTrait[-functionName]
	std::string getIdentifier() const;

	virtual std::string toString() const override {
		return "RoseConstruct: " + node->class_name() + ": " + node->unparseToString();
	}

	virtual std::string toDotString() const override {
		if (isSgFunctionDefinition(node)) {	// don't print whole function
			return isSgFunctionDefinition(node)->get_declaration()->get_name().getString();
		} else {
			std::string dotString(node->unparseToString());
			// escape " and \n (e.g. in string literals)
			boost::replace_all(dotString, "\n", "\\n");
			boost::replace_all(dotString, "\"", "\\\"");
			return dotString;
		}
	}

 private:
	SgNode* node;
};

class RoseFragment : public RoseConstruct {
 public:
	RoseFragment(SgNode* associatedNode, Sg_File_Info* info)
			: RoseConstruct(associatedNode, InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTFragment)),
				info(info) {}

	~RoseFragment() {}

	size_t getID() const { return (size_t)info; };
	Sg_File_Info* getFileInfo() { return info; }

	std::string toString() const override {
		std::stringstream ss;
		ss << "RoseFragment line:" << info->get_line() << " col:" << info->get_col();
		return ss.str();
	}

	std::string toDotString() const override {
		std::stringstream ss;
		ss << "line:" << info->get_line() << " col:" << info->get_col();
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

	std::shared_ptr<RoseConstruct> getFragment(SgNode* node, Sg_File_Info* fileInfo) {
		if (node == nullptr || fileInfo == nullptr) {
			throw std::string("RoseConstructProvider: attempted to getFragment for nullptr");
		}

		if (mapping.find(fileInfo) == mapping.end()) {
			logIt(DEBUG) << "\tcreating new construct" << std::endl;
			mapping[fileInfo] = std::make_shared<RoseFragment>(RoseFragment(node, fileInfo));
		}
		return mapping[fileInfo];
	}

	/** XXX this method does no checks on the SgNode! */
	std::shared_ptr<RoseConstruct> getConstruct(SgNode* node) {
		if (node == nullptr) {
			throw std::string("RoseConstructProvider: attempted to getConstruct for nullptr");
		}

		if (mapping.find(node) == mapping.end()) {
			logIt(DEBUG) << "\tcreating new construct" << std::endl;

			ConstructGenerator gen;
			node->accept(gen);
			mapping[node] = std::make_shared<RoseConstruct>(RoseConstruct(node, gen.getConstructTraits()));
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
