#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePFunctionScopeStrategy.h"

#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/CodeInsertionPointSelector.h"

#include <rose.h>


namespace InstRO {
	namespace Rose {
		namespace Adapter {
			namespace StrategyBasedAdapterSupport{


				ScorePFunctionScopeStrategy::ScorePFunctionScopeStrategy(ScorePInitializer* initializer)
					: GenericInstrumentationStrategy(initializer) {

				}


				ScorePFunctionScopeStrategy::~ScorePFunctionScopeStrategy() {

				}


				bool ScorePFunctionScopeStrategy::isImplementedFor(SgNode* node) {

					Sg_File_Info* fileInfo = node->get_file_info();
					SgScopeStatement* scope = SageInterface::getGlobalScope(node);

					// for now, restrict instrumentation to source files
					// TODO exclude system header functions in selector or handle here
					if (!fileInfo->isSameFile(scope->get_file_info())) return false;

					// consider base class restrictions
					if (!GenericInstrumentationStrategy::isImplementedFor(node)) return false;

					return isSgFunctionDefinition(node) || isSgFunctionDeclaration(node);
				}


				std::string ScorePFunctionScopeStrategy::regionName(SgNode* node) {

					SgFunctionDeclaration* fnDecl = SageInterface::getEnclosingFunctionDeclaration(node, true);

					// TODO identify overload?
					std::string name = fnDecl->get_qualified_name();
					std::stringstream postfix;

					postfix << "";

					return name + postfix.str();
				}


				std::string ScorePFunctionScopeStrategy::regionHandleName(SgNode* node) {

					std::map<SgNode*, std::string>::iterator i = nameHistory.find(node);

					if (i != nameHistory.end()) {
						return i->second;
					}

					std::string handleName = SageInterface::generateUniqueVariableName(SageInterface::getScope(node), "InstROHandle");

					nameHistory[node] = handleName;

					return handleName;
				}


				std::vector<SgDeclarationStatement*> ScorePFunctionScopeStrategy::constructDeclarationInMainFileFor(SgNode* node) {

					std::vector<SgDeclarationStatement*> result;

					SgFunctionDeclaration* mainfn = SageInterface::findMain(SageInterface::getProject());

					// construct handle, define handle in global scope

					SgGlobal* globalScope = SageInterface::getGlobalScope(mainfn);

					std::string handle = regionHandleName(node);

					// can't insert non-declaration statement into global scope,
					// even though they would be consumed by the preprocessor
					/*
					SgExpression* call = SageBuilder::buildFunctionCallExp(
					"SCOREP_USER_METRIC_GLOBAL",
					SageBuilder::buildVoidType(),
					SageBuilder::buildExprListExp(
					SageBuilder::buildOpaqueVarRefExp(handle, globalScope)),
					globalScope);

					SgStatement* decl = SageBuilder::buildExprStatement(call);
					*/

					// FIX: use the resolved form instead of preprocessor macro
					SgType* handleType = SageBuilder::buildOpaqueType("SCOREP_User_RegionHandle", globalScope);

					SgVarRefExp* assignValue = SageBuilder::buildOpaqueVarRefExp("SCOREP_USER_INVALID_REGION", globalScope);

					SgInitializer* initializer = SageBuilder::buildAssignInitializer(assignValue);

					SgVariableDeclaration* decl = SageBuilder::buildVariableDeclaration(handle, handleType, initializer, globalScope);

					result.push_back(decl);

					return result;
				}


				std::vector<SgDeclarationStatement*> ScorePFunctionScopeStrategy::constructDeclarationInLocalFileFor(SgNode* node) {

					std::vector<SgDeclarationStatement*> result;

					std::string handle = regionHandleName(node);

					// construct external reference in global scope of target node

					SgGlobal* globalScope = SageInterface::getGlobalScope(node);

					// can't insert non-declaration statement into global scope,
					// even though they would be consumed by the preprocessor
					/*
					call = SageBuilder::buildFunctionCallExp(
					"SCOREP_USER_METRIC_EXTERNAL",
					SageBuilder::buildVoidType(),
					SageBuilder::buildExprListExp(
					SageBuilder::buildOpaqueVarRefExp(handle, globalScope)),
					globalScope);

					SgExprStatement* extDecl = SageBuilder::buildExprStatement(call);
					*/

					SgType* handleType2 = SageBuilder::buildOpaqueType("SCOREP_User_RegionHandle", globalScope);

					SgVariableDeclaration* decl2 = SageBuilder::buildVariableDeclaration(handle, handleType2, NULL, globalScope);

					// Set declaration external, referencing defining declaration in main file (probably the same)
					SageInterface::setExtern(decl2);

					result.push_back(decl2);

					return result;
				}


				std::vector<SgStatement*> ScorePFunctionScopeStrategy::constructInitializationInMainScopeFor(SgNode* node) {

					std::vector<SgStatement*> result;

					std::string name = regionName(node);

					std::string handle = regionHandleName(node);

					std::string regionTypeName = "SCOREP_USER_REGION_TYPE_FUNCTION";

					SgBasicBlock* mainBody = SageInterface::findMain(SageInterface::getProject())->get_definition()->get_body();

					SgVarRefExp* arg1 = SageBuilder::buildOpaqueVarRefExp(handle, mainBody);
					SgStringVal* arg2 = SageBuilder::buildStringVal(name);
					SgVarRefExp* arg3 = SageBuilder::buildOpaqueVarRefExp(regionTypeName, mainBody);

					SgFunctionCallExp* call = SageBuilder::buildFunctionCallExp(
						"SCOREP_USER_REGION_INIT",
						SageBuilder::buildVoidType(),
						SageBuilder::buildExprListExp(arg1, arg2, arg3),
						mainBody);

					SgStatement* res = SageBuilder::buildExprStatement(call);

					result.push_back(res);

					return result;
				}


				std::vector<SgStatement*> ScorePFunctionScopeStrategy::constructEntryInstrumentationInsideNthScopeFor(SgScopeStatement* node, size_t index) {

					std::vector<SgStatement*> result;

					std::string handle = regionHandleName(node);

					SgExpression* call = SageBuilder::buildFunctionCallExp(
						"SCOREP_USER_REGION_ENTER",
						SageBuilder::buildVoidType(),
						SageBuilder::buildExprListExp(
						SageBuilder::buildOpaqueVarRefExp(handle, SageInterface::getGlobalScope(node))),
						SageInterface::getScope(node));

					SgStatement* res = SageBuilder::buildExprStatement(call);

					result.push_back(res);

					return result;
				}


				std::vector<SgStatement*> ScorePFunctionScopeStrategy::constructExitInstrumentationInsideNthScopeFor(SgScopeStatement* node, size_t index) {

					std::vector<SgStatement*> result;

					std::string handle = regionHandleName(node);

					SgExpression* call = SageBuilder::buildFunctionCallExp(
						"SCOREP_USER_REGION_END",
						SageBuilder::buildVoidType(),
						SageBuilder::buildExprListExp(
						SageBuilder::buildOpaqueVarRefExp(handle, SageInterface::getGlobalScope(node))),
						SageInterface::getScope(node));

					SgStatement* res = SageBuilder::buildExprStatement(call);

					result.push_back(res);

					return result;
				}


			}
		}
	}
}