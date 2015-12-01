#ifndef INSTRO_ROSE_TOOLING_ROSE_NAMED_CONSTRUCT_ACCESS_H
#define INSTRO_ROSE_TOOLING_ROSE_NAMED_CONSTRUCT_ACCESS_H

#include "instro/tooling/NamedConstructAccess.h"
#include "instro/core/ConstructSet.h"

#include <rose.h>

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace NamedConstructAccess {

/**
* \brief This class matches named entities of the AST with a given Matcher
*
* In this context it is crucial to know which nodes are regarded to have a meaningful name.
* We regard these nodes as named nodes:
* - A variable and its reference
* - A function defining definition node (this includes member functions)
* - A function reference contained in a function call
*
* \author Jan-Patrick Lehr, Christian Iwainsky, Roman Ness
*/
class NameMatchingASTTraversal : public ROSE_VisitorPatternDefaultBase {
 public:
	NameMatchingASTTraversal(InstRO::Tooling::NamedConstructAccess::Matcher* matcher)
			: matchingObject(matcher), cs(), csci(&cs) {}
	~NameMatchingASTTraversal() {}

	InstRO::Core::ConstructSet getResultingCS();

	void visit(SgFunctionDefinition* n);
	void visit(SgVarRefExp* n);
	void visit(SgFunctionRefExp* n);
	void visit(SgTemplateFunctionRefExp* n);
	void visit(SgMemberFunctionRefExp* n);
	void visit(SgTemplateMemberFunctionRefExp* n);
	void visit(SgNode* n);

 private:
	InstRO::Core::ConstructSet cs;
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface csci;
	InstRO::Tooling::NamedConstructAccess::Matcher* matchingObject;

	void handleFunctionRef(SgFunctionDeclaration* associatedDecl, SgExpression* funcRef);
};

class RoseNamedConstructAccess : public InstRO::Tooling::NamedConstructAccess::NamedConstructAccess {
 public:
	RoseNamedConstructAccess(SgProject* proj) : project(proj) {}

	InstRO::Core::ConstructSet getConstructsByIdentifierName(
			InstRO::Tooling::NamedConstructAccess::Matcher& matcher) override {

		NameMatchingASTTraversal gen(&matcher);
		for (auto n : SageInterface::querySubTree<SgLocatedNode>(project, V_SgLocatedNode)) {
			n->accept(gen);
		}
		return gen.getResultingCS();
	}

 private:
	SgProject* project;
};

}	// namespace NamedConstructAccess
}	// namespace Tooling
}	// namespace Rose
}	// namespace InstRO

#endif
