#include "instro/adapterbuilder/BackendInstrumentationAdapterBuilder.h"

using namespace InstRO;

BackendInstrumentationAdapterBuilder::BackendInstrumentationAdapterBuilder(Selector* selector, std::string attribute) :
		AdapterBuilder(selector) {
	this->compilerAttribute = attribute;
}
#if 0
void BackendInstrumentationAdapterBuilder::buildForNode(SgNode* node, Selector* decidingSelector) {

	if (this->decidingSelector != decidingSelector)
	return;

	if(isNodeSelected(node)) {
		if (isSgFunctionDeclaration(node)) {
#ifdef DEBUG
			std::cout << "Visiting function declaration for: " << isSgLocatedNode(node)->unparseToString() << std::endl;
#endif
			SageInterface::addTextForUnparser(isSgFunctionDeclaration(node)->get_firstNondefiningDeclaration(), compilerAttribute, AstUnparseAttribute::e_before);
		}
		if (isSgFunctionDefinition(node)) {
#ifdef DEBUG
			std::cout << "Visiting function definition for: " << isSgFunctionDefinition(node)->unparseToString() << std::endl;
#endif
			SageInterface::addTextForUnparser(isSgFunctionDefinition(node)->get_declaration()->get_firstNondefiningDeclaration(), compilerAttribute, AstUnparseAttribute::e_before);
		}
	}
}
}
#endif

void BackendInstrumentationAdapterBuilder::buildForNode(SgNode* node) {
	if (isNodeSelected(node)) {
		if (isSgFunctionDeclaration(node)) {
#ifdef DEBUG
			std::cout << "Visiting function declaration for: " << isSgLocatedNode(node)->unparseToString() << std::endl;
#endif
#if 0
			SageInterface::addTextForUnparser(isSgFunctionDeclaration(node)->get_firstNondefiningDeclaration(), compilerAttribute, AstUnparseAttribute::e_before);
#else
			isSgFunctionDeclaration(node)->get_functionModifier().setGnuAttributeNoInstrumentFunction();
			node->get_file_info()->setOutputInCodeGeneration();
			std::cout << "transformed Modifier: " << isSgFunctionDeclaration(node)->get_functionModifier() << std::endl;
#endif
		}
		if (isSgFunctionDefinition(node)) {
#ifdef DEBUG
			std::cout << "Visiting function definition for: " << isSgFunctionDefinition(node)->unparseToString() << std::endl;
			std::cout << isSgFunctionDefinition(node)->get_declaration() << std::endl;
#endif
#if 0
			SageInterface::addTextForUnparser(isSgFunctionDefinition(node)->get_declaration()->get_firstNondefiningDeclaration(), compilerAttribute, AstUnparseAttribute::e_before);
#else
			std::cout << "Old Modifier: " << isSgFunctionDefinition(node)->get_declaration()->get_functionModifier() << std::endl;
			isSgFunctionDefinition(node)->get_declaration()->get_functionModifier().setGnuAttributeNoInstrumentFunction();
			isSgFunctionDefinition(node)->get_declaration()->get_file_info()->setOutputInCodeGeneration();
			std::cout << "New Modifier: " << isSgFunctionDefinition(node)->get_declaration()->get_functionModifier() << std::endl;
#endif
		}
	}
}

void BackendInstrumentationAdapterBuilder::setCompilerSpecificAttribute(std::string attribute) {
	std::cout << "Warning: This will have no effect on the set gnu attribute at the moment." << std::endl;
	this->compilerAttribute = attribute;
}

std::string BackendInstrumentationAdapterBuilder::getCompilerSpecificAttribute() {
	return this->compilerAttribute;
}
