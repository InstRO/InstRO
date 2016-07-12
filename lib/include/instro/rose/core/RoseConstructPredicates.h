#ifndef INSTRO_ROSE_CORE_ROSE_CONSTRUCT_PREDICATES_H
#define INSTRO_ROSE_CORE_ROSE_CONSTRUCT_PREDICATES_H

namespace InstRO {
namespace Rose {
namespace Core {

namespace RoseConstructTraitPredicates {
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

		// Only have assign initializers if they lead to observable behavior
		if (isSgAssignInitializer(n)) {
			if (isSgCtorInitializerList(n->get_parent()->get_parent()) ||
					isSgEnumDeclaration(n->get_parent()->get_parent())) {
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

		if (isSgCastExp(n) || isSgAddressOfOp(n) || isSgCommaOpExp(n)) {
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
	bool operator()(SgNode* n) const override {
		if (isSgDoWhileStmt(n) || isSgWhileStmt(n) || isSgForStatement(n)) {
			return true;
		}
		return false;
	}
};

struct CTConditionalPredicate : public CTPredicate {
	bool operator()(SgNode* n) const override {
		if (isSgIfStmt(n) || isSgSwitchStatement(n)) {
			return true;
		}
		return false;
	}
};

struct CTScopeStatementPredicate : public CTPredicate {
	bool operator()(SgNode* n) const override {
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
	bool operator()(SgNode* n) const override { return isSgFunctionDefinition(n) != nullptr; }
};

struct CTFileScopePredicate : public CTPredicate {
	bool operator()(SgNode* n) const override { return isSgFile(n) != nullptr; }
};

struct CTGlobalScopePredicate : public CTPredicate {
	bool operator()(SgNode* n) const override { return isSgProject(n) != nullptr; }
};

struct CTSimpleStatementPredicate : public CTPredicate {
	bool operator()(SgNode* n) const override {
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

			if (isSgNullExpression(expr)) {
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

struct CTOpenMPPredicate : public CTPredicate {
	bool operator()(SgNode* n) const override { return isSgOmpBodyStatement(n) != nullptr; }
};

struct CTStatementPredicate : public CTPredicate {
	bool operator()(SgNode* n) const override {
		if (CTConditionalPredicate()(n) || CTLoopPredicate()(n) || CTScopeStatementPredicate()(n) ||
				CTSimpleStatementPredicate()(n) || CTOpenMPPredicate()(n)) {
			return true;
		}
		return false;
	}
};

struct CTWrappableStatementPredicate : public CTPredicate {
	bool operator()(SgNode* n) const override {
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
	bool operator()(SgNode* n) const override {
		return CTGlobalScopePredicate()(n) || CTFileScopePredicate()(n) || CTFunctionPredicate()(n) ||
					 CTStatementPredicate()(n) || CTExpressionPredicate()(n);
	}
};


//// TODO actually use that mechanism
std::unique_ptr<CTPredicate> getPredicateForTraitType(InstRO::Core::ConstructTraitType traitType);


} // Predicates
} // Core
} // Rose
} // InstRO

#endif
