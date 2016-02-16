#ifndef INSTRO_ROSE_CORE_CONSTRUCTSET_H_
#define INSTRO_ROSE_CORE_CONSTRUCTSET_H_

#include "instro/core/ConstructSet.h"
#include "instro/rose/utility/ASTHelper.h"
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
	virtual bool operator()(SgNode* n) const = 0;
	virtual ~CTPredicate() {}
};

struct DefinedVariableDeclarationPredicate {
	/// XXX that is so dumb, there has to be an easier way
	bool operator()(SgNode* n) const {
		auto initNames = isSgVariableDeclaration(n)->get_variables();
		return initNames[0]->get_initptr() != nullptr;
	}
};

struct ExpressionInLoopOrConditionalHeader {
	// either condition of IF or SWITCH
	// or condition of WHILE, DO-WHILE
	// also increment of FOR
	bool operator()(SgNode* n) const {
		if (isSgExpression(n)) {
			SgNode* parent = n->get_parent();
			if (parent == nullptr) {
				return false;
			}

			// null expression only allowed in for loop increment position
			if (isSgForStatement(n->get_parent()) && isSgForStatement(n->get_parent())->get_increment() == n) {
				return true;
			}
			if (isSgNullExpression(n)) {
				return false;
			}

			SgNode* grandParent = parent->get_parent();
			if (grandParent == nullptr) {
				return false;
			}

			if (isSgIfStmt(grandParent) && isSgIfStmt(grandParent)->get_conditional() == parent) {
				return true;

			} else if (isSgSwitchStatement(grandParent) &&
								 (isSgSwitchStatement(grandParent)->get_item_selector() == parent)) {
				return true;

			} else if (isSgDoWhileStmt(grandParent) && isSgDoWhileStmt(grandParent)->get_condition() == parent) {
				return true;

			} else if (isSgWhileStmt(grandParent) && isSgWhileStmt(grandParent)->get_condition() == parent) {
				return true;
			}
		}
		return false;
	}
};

struct IsStatementInForLoop {
	bool operator()(SgNode* n) const {
		SgNode* parent = n->get_parent();
		if (parent == nullptr) {
			return false;
		}

		if (isSgForStatement(parent) && isSgForStatement(parent)->get_test() == n) {
			return true;
		} else if (isSgForInitStatement(parent)) {
			return true;
		}

		return false;
	}
};

struct CTExpressionPredicate : public CTPredicate {
	bool operator()(SgNode* n) const override {
		if (isSgExprListExp(n) || isSgFunctionRefExp(n) || isSgMemberFunctionRefExp(n)) {
			return false;	// never accept these
		}
		if (isSgFunctionCallExp(n) != nullptr) {
			return true;	// always accept these
		}

		if (ExpressionInLoopOrConditionalHeader()(n)) {

			return true;
		} else if (isSgValueExp(n) || isSgVarRefExp(n) || isSgNullExpression(n)) {
			return false;
		}

		if (isSgAssignInitializer(n)) {
			if (isSgCtorInitializerList(n->get_parent()->get_parent())) {
				return false;
			}
			return true;
		}

		// do not allow initializers to show up in construct sets
		if (isSgConstructorInitializer(n)) {
			return false;
		}

		// function call expressions won't be affected
		if (isSgDotExp(n) || isSgThisExp(n) || isSgArrowExp(n)) {
			return false;
		}

		if (isSgCastExp(n) || isSgAddressOfOp(n)) {
			return false;
		}
		if (isSgUnaryOp(n) || isSgBinaryOp(n)) {
			if (!SageInterface::querySubTree<SgVarRefExp>(n, V_SgVarRefExp).empty() ||
					!SageInterface::querySubTree<SgFunctionCallExp>(n, V_SgFunctionCallExp).empty()) {
				return true;
			}
			return false;	// operators only containing literals of templateParameterVals are no expressions (-42)
		}

		return isSgExpression(n) != nullptr;
	}
};

struct CTLoopPredicate : public CTPredicate {
	bool operator()(SgNode* n) const {
		if (isSgDoWhileStmt(n) || isSgWhileStmt(n) || isSgForStatement(n)) {
			return true;
		}
		return false;
	}
};

struct CTConditionalPredicate : public CTPredicate {
	bool operator()(SgNode* n) const {
		if (isSgIfStmt(n) || isSgSwitchStatement(n)) {
			return true;
		}
		return false;
	}
};

struct CTScopeStatementPredicate : public CTPredicate {
	bool operator()(SgNode* n) const {

		if (!isSgBasicBlock(n)) {
			return false;
		}

		SgNode* parent = n->get_parent();
		if (isSgFunctionDefinition(parent)) {
			return false;	// ignore function scopes
		}
		if (isSgSwitchStatement(parent) || (isSgCaseOptionStmt(parent) && isSgLocatedNode(n)->isCompilerGenerated())) {
			// always ignore scopes of Switch-stmts
			// ignore compiler generated scopes around case-option-stmts
			return false;
		}
		return true;
	}
};

struct CTFunctionPredicate : public CTPredicate {
	bool operator()(SgNode* n) const { return isSgFunctionDefinition(n) != nullptr; }
};

struct CTFileScopePredicate : public CTPredicate {
	bool operator()(SgNode* n) const { return isSgFile(n) != nullptr; }
};

struct CTGlobalScopePredicate : public CTPredicate {
	bool operator()(SgNode* n) const { return isSgProject(n) != nullptr; }
};

struct CTSimpleStatementPredicate : public CTPredicate {
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

		if (isSgCaseOptionStmt(n) || isSgDefaultOptionStmt(n)) {
			return false;	// these are only labels
		}

		if (IsStatementInForLoop()(n)) {
			// allow null statements in loop header, but set file info
			if (isSgNullStatement(n)) {
				isSgLocatedNode(n)->set_file_info(isSgLocatedNode(n->get_parent())->get_file_info());
			}
			return true;
		}

		if (isSgExprStatement(n)) {
			SgExpression* expr = isSgExprStatement(n)->get_expression();

			if (ExpressionInLoopOrConditionalHeader()(expr)) {
				return false;	// if with an expression as conditional
			}

			if (isSgNullExpression(expr) ) {
				return false;	// statement consisting only of semicolon
			}

			return true;
		}

		if (isSgScopeStatement(n)) {
			return false;
		}

		if (isSgStatement(n)) {
			return !CTConditionalPredicate()(n) && !CTLoopPredicate()(n) && !CTFileScopePredicate()(n);
		}
		return false;
	}
};

struct CTStatementPredicate : public CTPredicate {
	bool operator()(SgNode* n) const {
		if (CTConditionalPredicate()(n) || CTLoopPredicate()(n) || CTScopeStatementPredicate()(n) ||
				CTSimpleStatementPredicate()(n)) {
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
		if (isSgBreakStmt(n) || isSgContinueStmt(n)) {
			return false;
		}
		return (isSgBasicBlock(n->get_parent()) != nullptr);
	}
};

struct ConstructPredicate : public CTPredicate {
	bool operator()(SgNode* n) const {
		return CTGlobalScopePredicate()(n) || CTFileScopePredicate()(n) || CTFunctionPredicate()(n) ||
					 CTStatementPredicate()(n) || CTExpressionPredicate()(n);
	}
};

//// TODO actually use that mechanism
std::unique_ptr<CTPredicate> getPredicateForTraitType(InstRO::Core::ConstructTraitType traitType);

}	// namespace RoseConstructLevelPredicates

class ConstructGenerator : public ROSE_VisitorPatternDefaultBase {
 public:
	ConstructGenerator() : ct(InstRO::Core::ConstructTraitType::CTNoTraits){};
	InstRO::Core::ConstructTrait getConstructTraits() { return ct; }

	// global scope
	void visit(SgProject* node) {
		if (RoseConstructLevelPredicates::CTGlobalScopePredicate()(node)) {
			ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTGlobalScope);
		} else {
			generateError(node);
		}
	}

	// file scope
	void visit(SgSourceFile* node) {
		if (RoseConstructLevelPredicates::CTFileScopePredicate()(node)) {
			ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTFileScope);
		} else {
			generateError(node);
		}
	}

	// function
	void visit(SgFunctionDefinition* node) {
		if (RoseConstructLevelPredicates::CTFunctionPredicate()(node)) {
			ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTFunction);
		} else {
			generateError(node);
		}
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
		if (RoseConstructLevelPredicates::CTScopeStatementPredicate()(node)) {
			ct = InstRO::Core::ConstructTrait(InstRO::Core::ConstructTraitType::CTScopeStatement);
			handleStatementWithWrappableCheck(node);
		} else {
			generateError(node);
		}
	}

	// statements
	void visit(SgStatement* node) {
		if (RoseConstructLevelPredicates::CTSimpleStatementPredicate()(node)) {
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
		if (RoseConstructLevelPredicates::CTExpressionPredicate()(node)) {
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
