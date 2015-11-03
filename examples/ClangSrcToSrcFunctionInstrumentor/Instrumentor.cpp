#include "llvm/Support/CommandLine.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/CommonOptionsParser.h"

#include "instro/clang/ClangInstrumentor.h"
#include "instro/utility/Logger.h"

/*
 * This is meant as something where one can simply look up
 * syntax and to check whether instantiations would compile :)
 */

static llvm::cl::OptionCategory MyTool("my tool");

int main(int argc, char **argv) {
	InstRO::logIt(InstRO::INFO) << "Start" << std::endl;

	InstRO::Clang::ClangInstrumentor instro(argc, const_cast<const char **>(argv), MyTool);

	auto fac = instro.getFactory();
	auto fDefSel = fac->createFunctionDefinitionSelector();

	fac->createCygProfileAdapter(fDefSel);
//	fac->createLLVMInputAdapter(fDefSel);

	instro.apply();

	InstRO::logIt(InstRO::INFO) << "End" << std::endl;
	return 0;
}
