#include "instro/clang/tooling/ClangConstructTraitInterface.h"

#include "instro/clang/core/ClangConstruct.h"

#include "clang/AST/RecursiveASTVisitor.h"

using namespace InstRO::Core;

namespace {

class ConstructCollector : public clang::RecursiveASTVisitor<ConstructCollector> {
 public:
	ConstructCollector(ConstructTraitType constructTrait, ConstructSet *constructs)
			: constructTrait(constructTrait), csci(constructs) {}

	bool shouldVisitTemplateInstantiations() { return true; }

	bool VisitDecl(clang::Decl *decl) {
		auto construct = std::make_shared<InstRO::Clang::Core::ClangConstruct>(decl);
		processConstruct(construct);

		return true;
	}

	bool VisitStmt(clang::Stmt *stmt) {
		auto construct = std::make_shared<InstRO::Clang::Core::ClangConstruct>(stmt);
		processConstruct(construct);

		return true;
	}

 private:
	ConstructTraitType constructTrait;
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface csci;

	void processConstruct(std::shared_ptr<InstRO::Clang::Core::ClangConstruct> construct) {
		if (construct->getTraits().is(constructTrait)) {
			csci.put(construct);
		}
	}
};
}

using namespace InstRO::Clang::Tooling;
using namespace InstRO::Clang::Tooling::ConstructTraitInterface;

ConstructSet ClangConstructTraitInterface::getConstructsByTrait(const ConstructTraitType constructTrait) {
	ConstructSet constructs;
	ConstructCollector collector(constructTrait, &constructs);
	collector.TraverseDecl(context.getTranslationUnitDecl());

	return constructs;
}
