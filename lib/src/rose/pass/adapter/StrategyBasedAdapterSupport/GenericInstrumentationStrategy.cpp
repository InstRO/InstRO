#include "instro/adapterbuilder/GenericInstrumentationStrategy.h"
#include "instro/utility/instrobuilder.h"

#include "instro/utility/CodeInsertionPointSelector.h"
#include "instro/utility/CodeInsertionHelper.h"


using namespace InstRO;

// helpers in anonymous local namespace
namespace {
typedef CodeInsertionHelper (*InsertionPointSelectorStmt) (SgStatement*);
typedef CodeInsertionHelper (*InsertionPointSelectorNode) (SgNode*);
typedef CodeInsertionHelper (*InsertionPointSelectorNthStmt) (SgStatement*, size_t);

template <typename _T1, typename _T2>
void _applyNodesToCodeInsertionHelper(_T1* n, const std::vector<_T2*>& v, CodeInsertionHelper (*sel)(_T1*)) {

	if (v.size()) {
		CodeInsertionHelper h = sel(n);
		for (auto i : v) {
			h.pushStatement(i);
		}
	}
}

// additional index parameter
void applyNodesToCodeInsertionHelper(SgStatement* n, const std::vector<SgStatement*>& v, InsertionPointSelectorNthStmt sel, size_t scopeIndex) {

	if (v.size()) {
		CodeInsertionHelper h = sel(n, scopeIndex);
		for (auto i : v) {
			h.pushStatement(i);
		}
	}
}

void applyNodesToCodeInsertionHelper(SgNode* n, const std::vector<SgStatement*>& v, InsertionPointSelectorNode sel) {
	_applyNodesToCodeInsertionHelper(n, v, sel);
}
void applyNodesToCodeInsertionHelper(SgStatement* n, const std::vector<SgStatement*>& v, InsertionPointSelectorStmt sel) {
	_applyNodesToCodeInsertionHelper(n, v, sel);
}
void applyNodesToCodeInsertionHelper(SgNode* n, const std::vector<SgStatement*>& v, InsertionPointSelectorStmt sel) {
	_applyNodesToCodeInsertionHelper(isSgStatement(n), v, sel);
}
void applyNodesToCodeInsertionHelper(SgNode* n, const std::vector<SgDeclarationStatement*>& v, InsertionPointSelectorNode sel) {
	_applyNodesToCodeInsertionHelper(n, v, sel);
}
void applyNodesToCodeInsertionHelper(SgStatement* n, const std::vector<SgDeclarationStatement*>& v, InsertionPointSelectorStmt sel) {
	_applyNodesToCodeInsertionHelper(n, v, sel);
}
void applyNodesToCodeInsertionHelper(SgNode* n, const std::vector<SgDeclarationStatement*>& v, InsertionPointSelectorStmt sel) {
	_applyNodesToCodeInsertionHelper(isSgStatement(n), v, sel);
}

} // end of anonymous namespace

GenericInstrumentationStrategy::GenericInstrumentationStrategy(ToolInitializer* toolInitializerStrategy)
	: toolInitializer(toolInitializerStrategy) {

}


GenericInstrumentationStrategy::~GenericInstrumentationStrategy() {

}


bool GenericInstrumentationStrategy::isImplementedFor(SgNode* node) {
	// generic implementation just returns generic enter and exit instrumentors without considering nodetype
	// if a function declaration is provided, the definition is required
	return !isSgFunctionDeclaration(node) || isSgFunctionDeclaration(node)->get_definition();
}


void GenericInstrumentationStrategy::applyInstrumentationFor(SgNode* node) {

	// always work on function definition, even if declaration is selected
	if (isSgFunctionDeclaration(node)) {
		node = isSgFunctionDeclaration(node)->get_definition();
		assert(node);
	}

	SgFunctionDeclaration* mainFn = SageInterface::findMain(SageInterface::getProject());

	SgBasicBlock* mainBody = mainFn->get_definition()->get_body();

	SgGlobal* global = SageInterface::getGlobalScope(node);

	// tool specific declarations
	applyNodesToCodeInsertionHelper(mainFn, constructToolDeclarationInMainFile(), &getEarliestGlobalDeclarationPoint);

	includeToolHeadersToMainFile();

	applyNodesToCodeInsertionHelper(node, constructToolDeclarationInLocalFile(global), &getEarliestGlobalDeclarationPoint);

	includeToolHeadersToLocalFile(global);


	// node specific
	applyNodesToCodeInsertionHelper(node, constructDeclarationInMainFileFor(node), &getMainGlobalDeclarationPoint);

	applyNodesToCodeInsertionHelper(node, constructDeclarationInLocalFileFor(node), &getNearestGlobalDeclarationPoint);

	applyNodesToCodeInsertionHelper(node, constructDeclarationInFunctionScopeFor(node), &getFunctionInitializationPoint);

	applyNodesToCodeInsertionHelper(node, constructInitializationInMainScopeFor(node), &getMainInitializationPoint);

	applyNodesToCodeInsertionHelper(node, constructInitializationInFunctionScopeFor(node), &getFunctionInitializationPoint);

	applyNodesToCodeInsertionHelper(node, constructInitializationLocallyFor(node), &getStatementEntryPoint);

	SgScopeStatement* scope = isSgScopeStatement(node);
	if (scope) {
		size_t innerScopeCount = 0;
		// Support distinct instrumentation for if/else or case-1/case-2/.../default scopes
		switch (scope->variantT()) {
			case V_SgIfStmt:
				innerScopeCount = 2;
				break;
			case V_SgSwitchStatement:
				innerScopeCount = SageInterface::getSwitchCases(isSgSwitchStatement(scope)).size();
			 	break;
			default:
				innerScopeCount = 1;
		}
		for (size_t i = 0; i < innerScopeCount; ++i) {
			applyNodesToCodeInsertionHelper(scope, constructExitInstrumentationInsideNthScopeFor(scope, i), &getNthInnerScopeExitPoints, i);
			applyNodesToCodeInsertionHelper(scope, constructEntryInstrumentationInsideNthScopeFor(scope, i), &getNthInnerScopeEntryPoint, i);
		}
	}

	applyNodesToCodeInsertionHelper(node, constructExitInstrumentationAfterNodeFor(node), &getStatementExitPoints);

	applyNodesToCodeInsertionHelper(node, constructEntryInstrumentationBeforeNodeFor(node), &getStatementEntryPoint);

	applyNodesToCodeInsertionHelper(mainBody, constructShutdownInMainFunctionFor(node), &getNthInnerScopeExitPoints, 0);

	// tool specific
	applyNodesToCodeInsertionHelper(node, constructToolInitializationInMainFunction(), &getMainSetupPoint);
	applyNodesToCodeInsertionHelper(mainBody, constructToolShutdownInMainFunction(), &getMainShutdownPoints);
}


std::vector<SgDeclarationStatement*> GenericInstrumentationStrategy::constructToolDeclarationInMainFile() {

	SgFile* f = SageInterface::getEnclosingFileNode(SageInterface::findMain(SageInterface::getProject()));
	return toolInitializer->constructToolDeclarationForMainFile(f);
}


void GenericInstrumentationStrategy::includeToolHeadersToMainFile() {

	SgFile* f = SageInterface::getEnclosingFileNode(SageInterface::findMain(SageInterface::getProject()));
	toolInitializer->includeToolHeadersToFile(f);
}


std::vector<SgDeclarationStatement*> GenericInstrumentationStrategy::constructToolDeclarationInLocalFile(SgGlobal* fileScope) {

	SgFile* f = SageInterface::getEnclosingFileNode(fileScope);
	return toolInitializer->constructToolDeclarationForLocalFile(f);
}


void GenericInstrumentationStrategy::includeToolHeadersToLocalFile(SgGlobal* fileScope) {

	SgFile* f = SageInterface::getEnclosingFileNode(fileScope);
	toolInitializer->includeToolHeadersToFile(f);
}


std::vector<SgStatement*> GenericInstrumentationStrategy::constructToolInitializationInMainFunction() {

	SgFunctionDeclaration* mainFn = SageInterface::findMain(SageInterface::getProject());
	SgBasicBlock* mainBody = mainFn->get_definition()->get_body();
	return toolInitializer->constructToolInitializationIn(mainBody);
}


std::vector<SgStatement*> GenericInstrumentationStrategy::constructToolShutdownInMainFunction() {

	SgFunctionDeclaration* mainFn = SageInterface::findMain(SageInterface::getProject());
	SgBasicBlock* mainBody = mainFn->get_definition()->get_body();
	return toolInitializer->constructToolShutdownIn(mainBody);
}


std::vector<SgDeclarationStatement*> GenericInstrumentationStrategy::constructDeclarationInMainFileFor(SgNode* n) {

	return std::vector<SgDeclarationStatement*>();
}


std::vector<SgDeclarationStatement*> GenericInstrumentationStrategy::constructDeclarationInLocalFileFor(SgNode* n) {

	return std::vector<SgDeclarationStatement*>();
}


std::vector<SgStatement*> GenericInstrumentationStrategy::constructDeclarationInFunctionScopeFor(SgNode* n) {

	return std::vector<SgStatement*>();
}


std::vector<SgStatement*> GenericInstrumentationStrategy::constructInitializationInMainScopeFor(SgNode* n) {

	return std::vector<SgStatement*>();
}


std::vector<SgStatement*> GenericInstrumentationStrategy::constructInitializationInFunctionScopeFor(SgNode* n) {

	return std::vector<SgStatement*>();
}


std::vector<SgStatement*> GenericInstrumentationStrategy::constructInitializationLocallyFor(SgNode* n) {

	return std::vector<SgStatement*>();
}


std::vector<SgStatement*> GenericInstrumentationStrategy::constructEntryInstrumentationBeforeNodeFor(SgNode* n) {

	return std::vector<SgStatement*>();
}


std::vector<SgStatement*> GenericInstrumentationStrategy::constructEntryInstrumentationInsideNthScopeFor(SgScopeStatement* node, size_t scopeIndex) {

	return std::vector<SgStatement*>();
}


std::vector<SgStatement*> GenericInstrumentationStrategy::constructExitInstrumentationAfterNodeFor(SgNode* n) {

	return std::vector<SgStatement*>();
}


std::vector<SgStatement*> GenericInstrumentationStrategy::constructExitInstrumentationInsideNthScopeFor(SgScopeStatement* node, size_t scopeIndex) {

	return std::vector<SgStatement*>();
}


std::vector<SgStatement*> GenericInstrumentationStrategy::constructShutdownInMainFunctionFor(SgNode* n) {

	return std::vector<SgStatement*>();
}


