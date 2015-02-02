
#ifndef CALLSITEADAPTERBUILDER_H_
#define CALLSITEADAPTERBUILDER_H_


#include "instro/common.h"
#include "instro/utility/instrobuilder.h"
#include "instro/utility/asthelper.h"

#include <set>
#include <string>

/**
 * \brief Insert instrumentation at call site.
 * \author roman
 */
class CallSiteAdapterBuilder : public AdapterBuilder {
public:
	CallSiteAdapterBuilder(Selector* decidingSelector);
	~CallSiteAdapterBuilder() {}

private:
	static const std::string FUNCTION_NAME_ENTER;
	static const std::string FUNCTION_NAME_EXIT;

protected:
	void buildForNode(SgNode* node);

	void modificationEnd();

private:
	bool instrumentationDeclared;
	std::set<SgFunctionCallExp*> callExprsToInstrument;

	void encloseWithInstrumentation(SgStatement* statement, SgFunctionCallExp* callExp);

	bool transformationNecessary(SgFunctionCallExp* callExp);
	void doTransformation(SgFunctionCallExp* callExp);

	SgExprStatement* createInstrumentationFunction(SgStatement* statement, std::string funcName,
			SgFunctionDefinition* calleeFuncDef, SgFunctionDefinition* callSiteFuncDef);

	SgExpression* buildInstrumentationArgument(SgFunctionDefinition* funcDef);

	void declareInstrumentationFunctions(SgStatement* statement);
	void buildInstrumentationFunction(SgScopeStatement* scope, std::string funcName);

	std::string getTmpVarName(SgFunctionCallExp* callExp);
};

#endif
