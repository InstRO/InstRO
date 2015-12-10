#ifndef INSTRO_CLANG_TOOLING_CLANG_ANALYSIS_INTERFACE_H
#define INSTRO_CLANG_TOOLING_CLANG_ANALYSIS_INTERFACE_H

#include "instro/tooling/AnalysisInterface.h"

#include "instro/clang/tooling/ClangConstructClassInterface.h"

namespace InstRO {
namespace Clang {
namespace Tooling {

class ClangAnalysisManager : public InstRO::Tooling::AnalysisManager {
 public:
 	ClangAnalysisManager(clang::ASTContext &context) : cci(new ConstructClassInterface::ClangConstructClassInterface(context)) {}
	ClangAnalysisManager() = delete;
	virtual ~ClangAnalysisManager() {}

	virtual InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph *getECG() override { return nullptr; }
	virtual InstRO::Tooling::ControlFlowGraph::ControlFlowGraph *getCFG() override { return nullptr; }
	virtual InstRO::Tooling::ConstructElevator::ConstructElevator *getCSElevator() override { return nullptr; }
	virtual InstRO::Tooling::ConstructClassInterface::ConstructClassInterface *getGrammarInterface() override {
		return cci.get();
	}
	virtual InstRO::Tooling::NamedConstructAccess::NamedConstructAccess *getNamedConstructAccessFacility() override {
		return nullptr;
	}

 protected:
	std::unique_ptr<ConstructClassInterface::ClangConstructClassInterface> cci;
};
}
}
}

#endif	// INSTRO_CLANG_TOOLING_CLANG_ANALYSIS_INTERFACE_H
