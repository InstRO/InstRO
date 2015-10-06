#include "llvm/Support/CommandLine.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/CommonOptionsParser.h"

#include "instro/clang/ClangInstrumentor.h"

/*
 * This is meant as something where one can simply look up
 * syntax and to check whether instantiations would compile :)
 */

static llvm::cl::OptionCategory MyTool("my tool");

int main(int argc, char **argv) {
	std::cout << "Start" << std::endl;

	InstRO::Clang::ClangInstrumentor instro(argc, const_cast<const char **>(argv), MyTool);

	auto fac = instro.getFactory();
	auto fDefSel = fac->createFunctionDefinitionSelector();
	std::vector<std::string> bList, wList;
	wList.push_back("foo");
	auto bwSel = fac->createBlackAndWhiteListSelector(bList, wList);
	auto bcSel = fac->createBooleanOrSelector(fDefSel, bwSel);

	//	fac->createCygProfileAdapter(fDefSel);
	fac->createLLVMInputAdapter(fDefSel);

	instro.apply();

	std::cout << "End" << std::endl;
	return 0;
}
