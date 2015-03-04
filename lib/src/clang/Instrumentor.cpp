#include "llvm/Support/CommandLine.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/CommonOptionsParser.h"

#include "instro/clang/core/ClangInstrumentor.h"

/*
 * This is meant as something where one can simply look up
 * syntax and to check whether instantiations would compile :)
 */


static llvm::cl::OptionCategory MyTool("InstRO");

int main(int argc, char **argv){
	clang::tooling::CommonOptionsParser cop(argc, const_cast<const char **>(argv), MyTool);
  clang::tooling::RefactoringTool tool(cop.getCompilations(), cop.getSourcePathList());
	::InstRO::Clang::ClangInstrumentor instro(NULL);
	tool.run(instro.getClangAction());
return 0;
}
