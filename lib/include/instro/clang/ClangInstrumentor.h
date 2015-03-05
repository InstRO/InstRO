#ifndef INSTRO_CLANG_CLANGINSTRUMENTOR_H
#define INSTRO_CLANG_CLANGINSTRUMENTOR_H

#include <memory>

#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/Refactoring.h"

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
class ClangInstrumentor : public ::InstRO::Instrumentor {
 public:
	/** CTor a user wants to use */
	ClangInstrumentor(int argc, const char** argv,
										::InstRO::Core::PassManagement::PassManager* manager);
	ClangInstrumentor(int argc, const char** argv);
	/** Provides the ToolAction, which a clang::RefactoringTool awaits */
	clang::tooling::ToolAction* getClangAction();
	/** Returns the factory used to create the passes a user builds the
	 * instrumentor with */
	InstRO::Core::PassFactory* getFactory(CompilationPhase phase = frontend);

	void init();
	void apply();
	void finalize();

 private:
	int argc;
	const char** argv;
	std::unique_ptr<InstRO::Clang::PassFactory> fac;
};
}	// Clang
}	// INstRO
#endif
