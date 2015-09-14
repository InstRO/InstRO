#include <instro/selectors/flopcountselector.h>
#include <iostream>

using namespace InstRO;

#define VERBOSE 1

/**
	* \brief Constructor, sets the operational threshold
	* \param threshold the minimum number of contained flops  to be selected
	*/
FlopCountSelector::FlopCountSelector(int threshold) { this->threshold = threshold; }

/**
	* \brief Is called on every node of the AST.
	* 	Counts floating-point operations for all function definitions.
	* \param start A node of the AST
	*/
void FlopCountSelector::visit(SgNode* start) {
	SgFunctionDefinition* funcDef = isSgFunctionDefinition(start);

	if (funcDef) {
		int flops = countByQuery(funcDef);
		flopsByFunc[funcDef] = flops;

#if VERBOSE
		std::cout << funcDef->get_declaration()->get_qualified_name() << " has " << flops << " flops." << std::endl;
#endif
	}
}

/**
	* \brief Counts floating-point operations in a function definition non-recursively
	* \param funcDef A function definition
	*/
int FlopCountSelector::countByQuery(SgFunctionDefinition* funcDef) {
	int count = 0;

	// XXX skip unary operations because they mostly contain dereferencing operators
	// get all binary operations of current function definition
	Rose_STL_Container<SgBinaryOp*> binOps = SageInterface::querySubTree<SgBinaryOp>(funcDef, V_SgBinaryOp);

	// TODO: does also contain assign operations // JP: Isn't there some isXYZ() to have some filter?
	// filter the list of binary operations
	Rose_STL_Container<SgBinaryOp*>::iterator it;
	for (it = binOps.begin(); it != binOps.end(); it++) {
		SgType* type = (*it)->get_type();
		if (type->isFloatType()) {
			count++;

#if VERBOSE
			std::cout << "  flop: " << (*it)->unparseToString() << "\ttype: " << SageInterface::get_name(*it) << std::endl;
#endif
		}
	}
	return count;
}

/**
	* \brief Select functions that have more floating-point operations than the threshold
	* \param project Top of the AST
	*/
void FlopCountSelector::selectionEnd(SgProject* project) {
	// XXX count recursively here? Call Graph? CFG?

	boost::unordered_map<SgFunctionDefinition*, int>::iterator it;
	for (it = flopsByFunc.begin(); it != flopsByFunc.end(); it++) {
		// select all functions with count >= threshold
		if (it->second >= threshold) {
			this->select(it->first);
		}
	}
}
