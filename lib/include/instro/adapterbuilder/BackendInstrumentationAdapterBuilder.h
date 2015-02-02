#ifndef BACKEND_COMPILER_INSTRUMENTATION_ADAPTER_BUILDER
#define BACKEND_COMPILER_INSTRUMENTATION_ADAPTER_BUILDER
#include "../common.h"

namespace InstRO {

/**
 * To do compiler based instrumentation this class uses the back end compiler's attribute/decl_spec mechanism.
 * The adapter builder places gcc's no_instrument_function on each node which is marked, so it will not be compiler instrumented through
 * gcc/g++.
 * The user can set and get the compiler specific attribute using the provided setters and getters.
 * \brief Marks all selected nodes with no_instrument_function attribute
 * \param the deciding selector
 * \param the string which should be added default is __attribute__((no_instrument_function))
 *
 * \fixme 2013-10-14 JP: If the function is the first member function in a class definition, the __attribute__ is placed before the visibility
 * specifier (public, private, protected) which leads to a gcc error. As I think this is not the intentino of the SageInterface and Unparser
 * it is a potential bug-report.
 *
 * \author Jan-Patrick Lehr
 */
class BackendInstrumentationAdapterBuilder : public AdapterBuilder {

public:
	BackendInstrumentationAdapterBuilder(Selector* selector, std::string attribute = "__attribute__ ((no_instrument_function))");
	void setCompilerSpecificAttribute(std::string attribute);
	std::string getCompilerSpecificAttribute();

	// 2013-10-21 JP: New AdapterBuilder interface
	void buildForNode(SgNode* node);

private:
	std::string compilerAttribute;


};
}

#endif
