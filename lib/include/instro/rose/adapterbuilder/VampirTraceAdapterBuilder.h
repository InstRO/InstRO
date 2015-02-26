#ifndef VAMPIR_TRACE_ADAPTER__
#define VAMPIR_TRACE_ADAPTER__

#include "GenericFunctionAdapterBuilder.h"
#include "../utility/functionidentification.h"

namespace InstRO {


/**
 * \brief This adapter builder inserts __cyg_profile_func_enter / exit like the CygProfileFunctionAdapterBuilder does but it uses "pseudo function ids" which are generated at compile time.
 *
 * After instrumentation has been done this adapter builder requires to call the writeNameFile() method, to create
 * the name file which then can be used by vampir to create the trace.
 * The adapter builder appends to the InstRO_nmfile and uses this file to check whether it should start numbering with an index of 1 or variable.
 * That way it can process more than one file, without overriding name mappings.
 * \ingroup Adapterbuilder
 * \author Jan-Patrick Lehr
 */
class VampirTraceAdapterBuilder: public GenericFunctionAdapterBuilder {
public:
	VampirTraceAdapterBuilder(Selector* decidingSelector);
	virtual ~VampirTraceAdapterBuilder();
	/** \brief Write the generated name file to disk */
	void writeNamefile();

protected:
	SgExprStatement* createEntryFunctionExpressionStatement(SgScopeStatement* scope);
	SgExprStatement* createExitFunctionExpressionStatement(SgScopeStatement* scope);
	void setUpFunctionDefinitionInstrumentationEnvironment(SgFunctionDefinition* funcDef);

private:
	bool writeCalled; // internally used marker

	FunctionIdentificationTable identifierTable;
};
}
#endif
