#ifndef INSTRO_CLANG_CLANGINSTRUMENTOR_H
#define INSTRO_CLANG_CLANGINSTRUMENTOR_H

#include "instro/Instrumentor.h"
#include "instro/clang/ClangPassFactory.h"
#include "instro/clang/tooling/ClangAnalysisInterface.h"

#include "llvm/Support/CommandLine.h"

#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring.h"

#include <memory>

namespace InstRO {
namespace Clang {

/**
 * In order to be runnable with Clang's refactoring tool this class has the
 * getClangConsumer() method, which will create the necessary instance and
 * return
 * to the caller
 */
class ClangInstrumentor : public InstRO::Instrumentor {
 public:
	ClangInstrumentor() = delete;
	ClangInstrumentor(int argc, const char** argv, llvm::cl::OptionCategory& llvmThing);
	/** Provides the ToolAction, which a clang::RefactoringTool awaits */
	clang::tooling::ToolAction* getClangAction();
	/** Returns the factory used to create the passes a user builds the
	 * instrumentor with */
	virtual InstRO::Clang::ClangPassFactory* getFactory(CompilationPhase phase = frontend) override;
	/** Accessor function for the (in general) compiler independent analysis layer */
	virtual InstRO::Tooling::AnalysisManager* getAnalysisManager() override;

	/// \brief Runs the Instrumentor and its associated passes.
	/// \note ClangConstruct instances are only valid during execution of this method because the AST nodes
	/// are automatically deallocated after the specified source code file has been processed.
	void virtual apply() override;

	/// Initializes the AnalysisManager using the provided clang::ASTContext.
	void initializeAnalysisManager(clang::ASTContext& context);

 protected:
	clang::tooling::RefactoringTool& getTool();

 private:
	int argc;
	const char** argv;
	clang::tooling::CommonOptionsParser cop;
	clang::tooling::RefactoringTool tool;
	std::unique_ptr<InstRO::Clang::ClangPassFactory> fac;
	std::unique_ptr<InstRO::Clang::Tooling::ClangAnalysisManager> cam;
};
}	// Clang
}	// InstRO
#endif
