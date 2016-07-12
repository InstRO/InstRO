#ifndef INSTRO_ROSE_CORE_CONSTRUCTSET_H_
#define INSTRO_ROSE_CORE_CONSTRUCTSET_H_

#include "instro/core/ConstructSet.h"
#include "instro/rose/utility/ASTHelper.h"
#include "instro/utility/Logger.h"

#include "rose.h"

#include <boost/algorithm/string.hpp>
#include <map>
#include <cassert>

namespace InstRO {
namespace Rose {
namespace Core {

class ConstructGenerator : public ROSE_VisitorPatternDefaultBase {
 public:
	ConstructGenerator() : ct(InstRO::Core::ConstructTraitType::CTNoTraits){};
	InstRO::Core::ConstructTrait getConstructTraits() { return ct; }

	// global scope
	void visit(SgProject* node);

	// file scope
	void visit(SgSourceFile* node);

	// function
	void visit(SgFunctionDefinition* node);

	// conditionals
	void visit(SgIfStmt* node);
	void visit(SgSwitchStatement* node);

	// loops
	void visit(SgForStatement* node);
	void visit(SgWhileStmt* node);
	void visit(SgDoWhileStmt* node);

	// scopes
	void visit(SgBasicBlock* node);

	// statements
	void visit(SgStatement* node);
	void visit(SgVariableDeclaration* node);
	
	// openmp
	void visit(SgOmpBarrierStatement* node);
	void visit(SgOmpBodyStatement* node);
	void visit(SgOmpFlushStatement* node);
	void visit(SgOmpTaskwaitStatement* node);
	
	// expressions
	void visit(SgExpression* node);

	// this should be an error
	void visit(SgScopeStatement* node) { generateError(node); }
	void visit(SgDeclarationStatement* node) { generateError(node); }
	void visit(SgNode* node) { generateError(node); }

 private:
	InstRO::Core::ConstructTrait ct;

	void handleStatementWithWrappableCheck(SgNode* node);

	void generateError(SgNode* node);
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

	virtual std::string toString() const override; 
	
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
		return "RoseFragment line: " + std::to_string(info->get_line()) + " col: " + std::to_string(info->get_col());
	}

	std::string toDotString() const override {
		return "line:" + std::to_string(info->get_line()) + " col:" + std::to_string(info->get_col());
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

	std::shared_ptr<InstRO::Core::Construct> getFragment(SgNode* node, Sg_File_Info* fileInfo);
	/**
	 * If there is no construct for a node a dummy construct is returned.
	 * It can be safely inserted into construct sets without effect.
	 */
	std::shared_ptr<InstRO::Core::Construct> getConstruct(SgNode* const node);

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
