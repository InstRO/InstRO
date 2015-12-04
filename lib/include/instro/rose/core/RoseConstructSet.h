#ifndef INSTRO_ROSE_CORE_CONSTRUCTSET_H_
#define INSTRO_ROSE_CORE_CONSTRUCTSET_H_

#include "instro/core/ConstructSet.h"
#include "instro/utility/Logger.h"

#include "rose.h"

#include <boost/algorithm/string.hpp>
#include <map>
#include <iostream>
#include <cassert>

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
	/// XXX that is so dumb, there has to be an easier way
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
		if (isSgValueExp(n) != nullptr || isSgVarRefExp(n) != nullptr) {
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

		if (isSgAssignInitializer(n)) {
			return false;
		}

		// null expression only allowed in for loop increment position
		if (isSgNullExpression(n)) {
			if(isSgForStatement(n->get_parent())){
				return true;
			}
			return false;
		}

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
		if (isSgDeclarationStatement(n)) {
			if (isSgVariableDeclaration(n) && DefinedVariableDeclarationPredicate()(n)) {
				return true;	// only variable declarations with initializer
			}
			return false;
		}

		if (isSgForInitStatement(n)) {
			return false;	// no equivalent in C/C++ semantics
		}

		if (isSgNullStatement(n) && (isSgForStatement(n->get_parent()) || isSgForInitStatement(n->get_parent()))) {
			// allow null statements in loop header, but set file info
			isSgLocatedNode(n)->set_file_info(isSgLocatedNode(n->get_parent())->get_file_info());
			return true;
		}

		if (isSgExprStatement(n)) {
			SgNode* parent = n->get_parent();
			if (isSgIfStmt(parent) || isSgWhileStmt(parent) || isSgDoWhileStmt(parent)) {	// TODO other cases?
				return false;	// if with an expression as conditional
			}
			return true;
		}

		if (isSgScopeStatement(n)) {
			return false;
		}

		if (isSgStatement(n)) {
			return !CLConditionalPredicate()(n) && !CLLoopPredicate()(n) && !CLFileScopePredicate()(n);
		}
		return false;
	}
};

struct CLStatementPredicate : public CTPredicate {
	bool operator()(SgNode* n) const {
		if (CLConditionalPredicate()(n) || CLLoopPredicate()(n) || CLScopeStatementPredicate()(n) ||
				CLSimpleStatementPredicate()(n)) {
			return true;
		}
		return false;
	}
};

struct CTWrappableStatementPredicate : public CTPredicate {
	bool operator()(SgNode* n) const {
		if (isSgReturnStmt(n)) {
			return false;	// return stmts are not wrappable
		}
		return (isSgBasicBlock(n->get_parent()) != nullptr);
	}
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
	void visit(SgExpression* node) {
		if (RoseConstructLevelPredicates::CLExpressionPredicate()(node)) {
			ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTExpression);
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

	void handleStatementWithWrappableCheck(SgNode* node) {
		ct.add(InstRO::Core::ConstructTraitType::CTStatement);
		if (RoseConstructLevelPredicates::CTWrappableStatementPredicate()(node)) {
			ct.add(InstRO::Core::ConstructTraitType::CTWrappableStatement);
		}
	}

	void generateError(SgNode* node) {
		ct = InstRO::Core::ConstructTraitType::CTNoTraits;
		logIt(INFO) << "ConstructGenerator: Skipped SgNode " << node->class_name() << "\t" << node->unparseToString()
								<< std::endl;
	}
};

class RoseConstruct : public InstRO::Core::Construct {
 public:
	RoseConstruct(SgNode* const sgnode, InstRO::Core::ConstructTrait traits)
			: InstRO::Core::Construct(traits), node(sgnode) {}
	virtual ~RoseConstruct() {}

	virtual size_t getID() const override { return reinterpret_cast<size_t>(node); }
	SgNode* getNode() const { return node; }

	// we construct the identification as follows:
	// filename:startline--ConstructTrait[-functionName]
	std::string getIdentifier() const;

	virtual std::string toString() const override {
		std::string className = node->class_name();
		std::string unparse;

		if (getTraits().is(InstRO::Core::ConstructTraitType::CTFunction)) {
			// get only name of function
			unparse = isSgFunctionDefinition(node)->get_declaration()->get_qualified_name().getString();
		} else if (getTraits().is(InstRO::Core::ConstructTraitType::CTFileScope)) {
			// get only name of file
			unparse = isSgSourceFile(node)->getFileName();
		} else if (getTraits().is(InstRO::Core::ConstructTraitType::CTGlobalScope)) {
			unparse = std::string("GlobalScope");
		} else {
			unparse = node->unparseToString();
			// escape " and \n (e.g. in string literals)
		}
		return className + std::string(" | ") + unparse;
	}

	virtual std::string toDotString() const override {
		std::string resultString = this->toString();
		boost::replace_all(resultString, "\n", "\\n");
		boost::replace_all(resultString, "\"", "\\\"");
		return resultString;
	}


 protected:
	std::string specificConstructClassToString() const;
	virtual int determineCorrectLineInfo() const;
	virtual int determineCorrectColumnInformation() const;
	std::string determineCorrectFilename() const;

 private:
	SgNode* const node;
};

class RoseFragment : public RoseConstruct {
 public:
	RoseFragment(SgNode* associatedNode, Sg_File_Info* info)
			: RoseConstruct(associatedNode, InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTFragment)),
				info(info) {}

	~RoseFragment() {}

	size_t getID() const override { return reinterpret_cast<size_t>(info); };
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

 protected:
	virtual int determineCorrectLineInfo() const override;
	virtual int determineCorrectColumnInformation() const override;


 private:
	Sg_File_Info* const info;
};

class RoseConstructProvider {
 public:
	static RoseConstructProvider& getInstance() {
		static RoseConstructProvider instance;
		return instance;
	}

	std::shared_ptr<InstRO::Core::Construct> getFragment(SgNode* node, Sg_File_Info* fileInfo) {
		if (node == nullptr || fileInfo == nullptr) {
			throw std::string("RoseConstructProvider: attempted to getFragment for nullptr");
		}

		if (mapping.find(fileInfo) == mapping.end()) {
			logIt(DEBUG) << "\tcreating new construct" << std::endl;
			mapping[fileInfo] = std::make_shared<RoseFragment>(RoseFragment(node, fileInfo));
		}
		return mapping[fileInfo];
	}

	/**
	 * If there is no construct for a node a dummy construct is returned.
	 * It can be safely inserted into construct sets without effect.
	 */
	std::shared_ptr<InstRO::Core::Construct> getConstruct(SgNode* const node) {
		if (node == nullptr) {
			throw std::string("RoseConstructProvider: attempted to getConstruct for nullptr");
		}

		if (mapping.find(node) == mapping.end()) {
			logIt(DEBUG) << "\tcreating new construct" << std::endl;

			ConstructGenerator gen;
			node->accept(gen);

			if (gen.getConstructTraits().is(InstRO::Core::ConstructTraitType::CTNoTraits)) {
				mapping[node] = InstRO::Core::DummyConstruct::getInstance();
			} else {
				mapping[node] = std::make_shared<RoseConstruct>(RoseConstruct(node, gen.getConstructTraits()));
			}
		}
		return mapping[node];
	}

 private:
	std::map<SgNode* const, std::shared_ptr<InstRO::Core::Construct> > mapping;

 private:
	RoseConstructProvider(){};
	RoseConstructProvider(RoseConstructProvider&) = delete;
	void operator=(RoseConstructProvider const&) = delete;
};

}	// namespace Core

std::shared_ptr<InstRO::Rose::Core::RoseConstruct> toRoseConstruct(std::shared_ptr<InstRO::Core::Construct> c);

}	// namespace Rose
}	// Namespace InstRO

#endif /* INSTRO_ROSE_CORE_CONSTRUCTSET_H_ */
