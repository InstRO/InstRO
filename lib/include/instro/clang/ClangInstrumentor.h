#ifndef INSTRO_CLANG_CLANGINSTRUMENTOR_H
#define INSTRO_CLANG_CLANGINSTRUMENTOR_H

#include <memory>

#include "llvm/Support/CommandLine.h"

#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/Refactoring.h"

#include "instro/core/SimplePassManager.h"
#include "instro/core/Instrumentor.h"
#include "instro/clang/core/ClangConsumerFactory.h"
#include "instro/clang/core/PassFactory.h"

namespace InstRO {
namespace Clang {

/*
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
	InstRO::Clang::PassFactory* getFactory(CompilationPhase phase = frontend);
	/** Accessor function for the (in general) compiler independent analysis layer */
	virtual Tooling::AnalysisManager* getAnalysisManager() override { return nullptr; };

	void init();
	void apply();
	void finalize();

 private:
	int argc;
	const char** argv;
	clang::tooling::CommonOptionsParser cop;
	clang::tooling::RefactoringTool tool;
	std::unique_ptr<InstRO::Clang::PassFactory> fac;
};
}	// Clang
}	// InstRO
#endif
