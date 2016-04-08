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
* Named nodes can be
* - A variable reference -> collect all enclosing expressions
* - A function definition node -> collect the definition
* - A function reference -> collect the function call expression
*
* \author Roman Ness
*/
class NameMatchingASTTraversal : public ROSE_VisitorPatternDefaultBase {
 public:
	NameMatchingASTTraversal(InstRO::Tooling::NamedConstructAccess::Matcher* matcher)
			: matchingObject(matcher), cs(), csci(&cs) {}
	~NameMatchingASTTraversal() {}

	InstRO::Core::ConstructSet getResultingCS();

	void visit(SgFunctionDefinition* n) override;
	void visit(SgVarRefExp* n) override;
	void visit(SgFunctionRefExp* n) override;
	void visit(SgTemplateFunctionRefExp* n) override;	// XXX does not occur in ast?
	void visit(SgMemberFunctionRefExp* n) override;
	void visit(SgTemplateMemberFunctionRefExp* n) override;	// XXX does not occur in ast?
	void visit(SgVariableDeclaration* n) override;
	void visit(SgNode* n) override;

 private:
	InstRO::Core::ConstructSet cs;
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface csci;
	InstRO::Tooling::NamedConstructAccess::Matcher* matchingObject;

	void handleFunctionRef(SgFunctionDeclaration* associatedDecl, SgExpression* funcRef);
};

/**
 * \author Roman Ness
 * XXX The visitor traversal is a bit awkward. There has to be an easier way in rose
 */
class RoseNamedConstructAccess : public InstRO::Tooling::NamedConstructAccess::NamedConstructAccess {
 public:
	RoseNamedConstructAccess(SgProject* proj) : project(proj) {}

	InstRO::Core::ConstructSet getConstructsByIdentifierName(
			InstRO::Tooling::NamedConstructAccess::Matcher& matcher) override {
		NameMatchingASTTraversal visitor(&matcher);
		for (auto n : SageInterface::querySubTree<SgLocatedNode>(project, V_SgLocatedNode)) {
			n->accept(visitor);
		}
		return visitor.getResultingCS();
	}

 private:
	SgProject* project;
};

}	// namespace NamedConstructAccess
}	// namespace Tooling
}	// namespace Rose
}	// namespace InstRO

#endif
