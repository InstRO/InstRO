#ifndef INSTRO_CLANG_TOOLING_CLANG_ANALYSIS_INTERFACE_H
#define INSTRO_CLANG_TOOLING_CLANG_ANALYSIS_INTERFACE_H

#include "instro/tooling/AnalysisInterface.h"

#include "instro/clang/tooling/ClangConstructTraitInterface.h"

namespace InstRO {
namespace Clang {
namespace Tooling {

class ClangAnalysisManager : public InstRO::Tooling::AnalysisManager {
 public:
	ClangAnalysisManager(clang::ASTContext &context)
			: cti(new ConstructTraitInterface::ClangConstructTraitInterface(context)) {}
	ClangAnalysisManager() = delete;
	virtual ~ClangAnalysisManager() {}

	virtual InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph *getECG() override { return nullptr; }
	virtual InstRO::Tooling::ControlFlowGraph::ControlFlowGraph *getCFG() override { return nullptr; }
	virtual InstRO::Tooling::ConstructElevator::ConstructElevator *getCSElevator() override { return nullptr; }
	virtual InstRO::Tooling::ConstructTraitInterface::ConstructTraitInterface *getConstructTraitInterface() override {
		return cti.get();
	}
	virtual InstRO::Tooling::NamedConstructAccess::NamedConstructAccess *getNamedConstructAccessFacility() override {
		return nullptr;
	}

 protected:
	std::unique_ptr<ConstructTraitInterface::ClangConstructTraitInterface> cti;
};
}
}
}

#endif	// INSTRO_CLANG_TOOLING_CLANG_ANALYSIS_INTERFACE_H
