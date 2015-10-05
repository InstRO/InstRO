#include <iostream>
#include <fstream>

#include "instro/adapterbuilder/FunctionPrinterAdapterBuilder.h"
#include "instro/utility/asthelper.h"

using namespace InstRO;

FunctionPrinterAdapterBuilder::FunctionPrinterAdapterBuilder(Selector* sel, bool printToFile)
		: GenericFunctionAdapterBuilder(sel), decidingSelector(sel), printToFile(printToFile) {}

FunctionPrinterAdapterBuilder::~FunctionPrinterAdapterBuilder() {}

void FunctionPrinterAdapterBuilder::buildForNode(SgNode* node) {
	if (isSgFunctionDeclaration(node) || isSgFunctionDefinition(node) || isSgFunctionRefExp(node)) {
		std::ostream* out;
		if (printToFile) {
			// print to file
			out = new std::ofstream("marked_nodes.txt", std::ofstream::app);
		} else {
			// print to shell
			out = &std::cout;
		}

		if (isSgFunctionDefinition(node)) {
			SgFunctionDefinition* fDef = isSgFunctionDefinition(node);
			SgFunctionDeclaration* fDec = fDef->get_declaration();
			*out << "#FP#  " << ASTHelper::nondefUnparseToString(fDef) << "  [def]" << std::endl
					 << "  in \t" << fDec->getFilenameString() << std::endl
					 << std::endl;
		}

		if (isSgFunctionDeclaration(node)) {
			SgFunctionDeclaration* fDec = isSgFunctionDeclaration(node);
			*out << "#FP#  " << fDec->unparseToString() << "  [decl]" << std::endl
					 << "  in \t" << fDec->getFilenameString() << std::endl
					 << std::endl;
		}

		if (isSgFunctionRefExp(node)) {
			SgFunctionDeclaration* fDec = isSgFunctionRefExp(node)->getAssociatedFunctionDeclaration();
			*out << "#FP#  " << node->unparseToString() << "  [refExps]" << std::endl
					 << "  in \t" << fDec->getFilenameString() << std::endl
					 << std::endl;
		}
	}
}

void FunctionPrinterAdapterBuilder::buildForNode(SgNode* node, Selector* decidingSelector) {
	if (this->decidingSelector != decidingSelector)
		return;

	buildForNode(node);
}
