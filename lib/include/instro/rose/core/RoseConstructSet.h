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

			struct CLExpressionPredicate{
				bool operator()(SgNode * n) const
				{
					if (isSgExpression(n) != nullptr) return true;
					return false;
				}
			};

			struct CLStatementPredicate{

				bool operator()(SgNode * n) const
				{
					if (isSgFunctionDefinition(n) || isSgFunctionDeclaration(n)) return false;
					// out basic block of the function. it is equivalent to the function
					if (isSgBasicBlock(n) && isSgFunctionDefinition(n->get_parent())) return false;
					if (isSgVariableDeclaration(n) && isSgVariableDeclaration(n)->get_definition() != NULL) return true;
					if (isSgStatement(n) != nullptr) return true;
					return false;
				}
			};

			struct CLLoopPredicate{
				bool operator()(SgNode * n) const
				{
					if (isSgDoWhileStmt(n) != nullptr) return true;
					if (isSgWhileStmt(n) != nullptr) return true;
					if (isSgForStatement(n) != nullptr) return true;
					return false;
				}
			};

			struct CLConditionalPredicate{
				bool operator()(SgNode * n) const
				{
					if (isSgIfStmt(n) != nullptr) return true;
					if (isSgSwitchStatement(n) != nullptr) return true;
					return false;
				}
			};

			struct CLScopePredicate{
				bool operator()(SgNode * n) const
				{
					if (isSgBasicBlock(n) != nullptr) return true;
					return false;
				}
			};



			struct CLFunctionPredicate{
				bool operator()(SgNode * n) const
				{
					if (isSgFunctionDefinition(n) != nullptr) return true;
					return false;
				}
			};

			struct CLFileScopePredicate{
				bool operator()(SgNode * n) const
				{
					if (isSgFile(n) != nullptr) return true;
					return false;
				}
			};

			struct CLGlobalScopePredicate{
				bool operator()(SgNode * n) const
				{
					if (isSgGlobal(n) != nullptr) return true;
					return false;
				}
			};

			struct CLSimplePredicate{
				bool operator()(SgNode * n) const
				{
					if (!CLStatementPredicate()(n)) return false;
					if (CLFunctionPredicate()(n)) return false;
					if (CLScopePredicate()(n)) return false;
					if (CLConditionalPredicate()(n)) return false;
					if (CLLoopPredicate()(n)) return false;
					return true;
				}
			};


		struct InstrumentableConstructPredicate{
			bool operator()(SgNode * n) const;
		};
		/*
		struct InstrumentableConstructPredicate{
		bool operator()(SgNode * n) const
		{
		if (isSgDoWhileStmt(n) ||
		isSgBasicBlock(n) ||
		isSgFunctionDefinition(n)
		)
		return true;
		if (isSgExpression(n) != nullptr) return true;
		return false;
		}
		};
		*/
	
	}

class ConstructGenerator : public ROSE_VisitorPatternDefaultBase {
 public:
	enum ConstructLevel {
		EVERYTHING = 0,
		GLOBAL_SCOPE = 1,
		FILE_SCOPE = 2,
		FUNCTION = 3,
		STATEMENT = 4,
		EXPRESSION = 5,
		FRAGMENT = 6
	};

	enum StatementFlavor { NO_STATEMENT = 10, SIMPLE_STATEMENT = 11, LOOP = 12, CONDITIONAL = 13, SCOPE = 14 };

	ConstructGenerator() : level(ConstructLevel::EVERYTHING), flavor(StatementFlavor::NO_STATEMENT){};

	ConstructLevel getLevel() { return level; }
	StatementFlavor getFlavor() { return flavor; }
	InstRO::Core::ConstructLevelType getCLT() { return cl; }

	// TODO global scope
	// TODO file scope

	// function
	void visit(SgFunctionDefinition* node) {
		level = ConstructLevel::FUNCTION;
		cl = InstRO::Core::ConstructLevelType::CLFunction;
	}

	// conditionals
	void visit(SgIfStmt* node) {
		level = ConstructLevel::STATEMENT;
		flavor = StatementFlavor::CONDITIONAL;
		cl = InstRO::Core::ConstructLevelType::CLConditionalStatement;
	}
	void visit(SgSwitchStatement* node) {
		level = ConstructLevel::STATEMENT;
		flavor = StatementFlavor::CONDITIONAL;
		cl = InstRO::Core::ConstructLevelType::CLConditionalStatement;
	}

	// loops
	void visit(SgForStatement* node) {
		level = ConstructLevel::STATEMENT;
		flavor = StatementFlavor::LOOP;
		cl = InstRO::Core::ConstructLevelType::CLLoopStatement;
	}
	void visit(SgWhileStmt* node) {
		level = ConstructLevel::STATEMENT;
		flavor = StatementFlavor::LOOP;
		cl = InstRO::Core::ConstructLevelType::CLLoopStatement;
	}
	void visit(SgDoWhileStmt* node) {
		level = ConstructLevel::STATEMENT;
		flavor = StatementFlavor::LOOP;
		cl = InstRO::Core::ConstructLevelType::CLLoopStatement;
	}

	// scopes
	void visit(SgBasicBlock* node) {
		level = ConstructLevel::STATEMENT;
		flavor = StatementFlavor::SCOPE;
		cl = InstRO::Core::ConstructLevelType::CLScopeStatement;
	}

	// statements
	// TODO: any other statements that are not simple?
	void visit(SgStatement* node) {
		level = ConstructLevel::STATEMENT;
		flavor = StatementFlavor::SIMPLE_STATEMENT;
		cl = InstRO::Core::ConstructLevelType::CLSimpleStatement;
	}

	// expressions
	void visit(SgExpression* node) {
		level = ConstructLevel::EXPRESSION;
		cl = InstRO::Core::ConstructLevelType::CLExpression;
	}
	// CI: an initialized variable declaration is OK, 
	void visit(SgVariableDeclaration *n){
		if (n->get_definition()){
			level = ConstructLevel::STATEMENT;
                flavor = StatementFlavor::SIMPLE_STATEMENT;
                cl = InstRO::Core::ConstructLevelType::CLSimpleStatement;
}
	else generateError(n); }
	

	// this should be an error
	void visit(SgScopeStatement* node) { generateError(node); }
	void visit(SgDeclarationStatement* node) { generateError(node); }
	void visit(SgNode* node) { generateError(node); }

 private:
	ConstructLevel level;
	StatementFlavor flavor;
	InstRO::Core::ConstructLevelType cl;

	void generateError(SgNode* node) {
		std::cout << "# Encountered error case in ConstructGenerator. " << node->class_name()
							<< "\t" << node->unparseToString() << std::endl;
	}
};

class RoseConstruct : public InstRO::Core::Construct {
 public:
	RoseConstruct(SgNode* sgnode) : Construct(InstRO::Core::ConstructLevelType::CLNotALevel), node(sgnode) {
		if (sgnode==nullptr)
		{
			setLevel(InstRO::Core::ConstructLevelType::CLNotALevel);
		}
		else {
		ConstructGenerator gen;
		node->accept(gen);
		/*
		level = gen.getLevel();
		flavor = gen.getFlavor();*/
		this->setLevel(gen.getCLT());
		}
	}

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
		if (mapping.find(node) != mapping.end()) {
			return mapping[node];
		}
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
