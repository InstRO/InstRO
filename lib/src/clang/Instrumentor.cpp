#include "llvm/support/CommandLine.h"
#include "clang/Tooling/Refactoring.h"

#include "instro/clang/core/ClangInstrumentor.h"

/*
 * This is meant as something where one can simply look up
 * syntax and to check whether instantiations would compile :)
 */


static llvm::cl::OptionCategory MyTool("InstRO");

int main(int argc, char ** argv){
	clang::tooling::CommonOptionsParser cop(argc, argv, MyTool);
  clang::tooling::RefactoringTool tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());
	ClangInstrumentor instro;
	tool.run(instro.getClangConsumer());
return 0;
}
