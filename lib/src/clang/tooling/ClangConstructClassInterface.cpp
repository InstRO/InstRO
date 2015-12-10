#include "instro/clang/tooling/ClangConstructClassInterface.h"

#include "instro/clang/core/ClangConstruct.h"

#include "clang/AST/RecursiveASTVisitor.h"

using namespace InstRO::Core;

namespace {

class ConstructCollector : public clang::RecursiveASTVisitor<ConstructCollector> {

public:
	ConstructCollector(ConstructTraitType constructClass, ConstructSet *constructs) : constructClass(constructClass), csci(constructs) {}

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
	ConstructTraitType constructClass;
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface csci;

	void processConstruct(std::shared_ptr<InstRO::Clang::Core::ClangConstruct> construct) {
		if (construct->getTraits().is(constructClass)) {
			csci.put(construct);
		}
	}

};

}

using namespace InstRO::Clang::Tooling;
using namespace InstRO::Clang::Tooling::ConstructClassInterface;

ConstructSet ClangConstructClassInterface::getConstructsByClass(const ConstructTraitType constructClass) {
	ConstructSet constructs;
	ConstructCollector collector(constructClass, &constructs);
	collector.TraverseDecl(context.getTranslationUnitDecl());

	return constructs;
}
