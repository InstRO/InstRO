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
		cl = InstRO::Core::ConstructLevelType::CLConditional;
	}
	void visit(SgSwitchStatement* node) {
		level = ConstructLevel::STATEMENT;
		flavor = StatementFlavor::CONDITIONAL;
		cl = InstRO::Core::ConstructLevelType::CLConditional;
	}

	// loops
	void visit(SgForStatement* node) {
		level = ConstructLevel::STATEMENT;
		flavor = StatementFlavor::LOOP;
		cl = InstRO::Core::ConstructLevelType::CLLoop;
	}
	void visit(SgWhileStmt* node) {
		level = ConstructLevel::STATEMENT;
		flavor = StatementFlavor::LOOP;
		cl = InstRO::Core::ConstructLevelType::CLLoop;
	}
	void visit(SgDoWhileStmt* node) {
		level = ConstructLevel::STATEMENT;
		flavor = StatementFlavor::LOOP;
		cl = InstRO::Core::ConstructLevelType::CLLoop;
	}

	// scopes
	void visit(SgBasicBlock* node) {
		level = ConstructLevel::STATEMENT;
		flavor = StatementFlavor::SCOPE;
		cl = InstRO::Core::ConstructLevelType::CLScope;
	}

	// statements
	// TODO: any other statements that are not simple?
	void visit(SgStatement* node) {
		level = ConstructLevel::STATEMENT;
		flavor = StatementFlavor::SIMPLE_STATEMENT;
		cl = InstRO::Core::ConstructLevelType::CLSimple;
	}

	// expressions
	void visit(SgExpression* node) {
		level = ConstructLevel::EXPRESSION;
		cl = InstRO::Core::ConstructLevelType::CLExpression;
	}

	// this should be an error
	void visit(SgScopeStatement* node) { generateError(node); }
	void visit(SgDeclarationStatement* node) { generateError(node); }
	void visit(SgNode* node) { generateError(node); }

 private:
	ConstructLevel level;
	StatementFlavor flavor;
	InstRO::Core::ConstructLevelType cl;

	void generateError(SgNode* node) {
		std::cout << "# Encountered error case in ConstructGenerator."
							<< "\t" << node->unparseToString() << std::endl;
	}
};

class RoseConstruct : public InstRO::Core::Construct {
 public:
	RoseConstruct(SgNode* sgnode) : Construct(InstRO::Core::ConstructLevelType::CLNotALevel), node(sgnode) {
		ConstructGenerator gen;
		gen.visit(node);
		/*
		level = gen.getLevel();
		flavor = gen.getFlavor();*/
		this->setLevel(gen.getCLT());
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