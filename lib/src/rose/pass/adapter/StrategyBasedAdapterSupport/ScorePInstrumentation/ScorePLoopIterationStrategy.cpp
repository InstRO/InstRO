#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePLoopIterationStrategy.h"

#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/CodeInsertionPointSelector.h"

#include <rose.h>
// not included via rose.h
#include <AstRestructure.h>


namespace InstRO {
	namespace Rose {
		namespace Adapter {
			namespace StrategyBasedAdapterSupport{


				ScorePLoopIterationStrategy::ScorePLoopIterationStrategy(ScorePInitializer* initializer)
					: GenericInstrumentationStrategy(initializer) {

				}


				ScorePLoopIterationStrategy::~ScorePLoopIterationStrategy() {

				}


				bool ScorePLoopIterationStrategy::isImplementedFor(SgNode* node) {

					Sg_File_Info* fileInfo = node->get_file_info();
					SgScopeStatement* scope = SageInterface::getGlobalScope(node);

					// for now, restrict instrumentation to source files
					// TODO exclude system header functions in selector or handle here
					if (!fileInfo->isSameFile(scope->get_file_info())) return false;

					// consider base class restrictions
					if (!GenericInstrumentationStrategy::isImplementedFor(node)) return false;

					return isLoopStatement(node) || isLoopStatement(node->get_parent());
				}


				bool ScorePLoopIterationStrategy::isLoopStatement(SgNode* node) {

					switch (node->variantT()) {

					case V_SgForStatement:
					case V_SgWhileStmt:
					case V_SgDoWhileStmt:
						return true;

					default:
						return false;
					}
				}


				std::string ScorePLoopIterationStrategy::loopName(SgNode* node) {

					switch (node->variantT()) {

					case V_SgForStatement:
						return "for";

					case V_SgWhileStmt:
						return "while";

					case V_SgDoWhileStmt:
						return "do-while";

					default:
						return "";
					}
				}


				int ScorePLoopIterationStrategy::loopNodeIndex(SgNode* node) {

					VariantT variant = node->variantT();

					SgNode* base = SageInterface::getEnclosingFunctionDeclaration(node);

					// Traversal to find all loops of same type (eg. all for-loops) and assign them a number
					std::vector<SgNode*> nodes = SageInterface::querySubTree<SgNode>(base, variant);

					std::vector<SgNode*>::iterator it = std::find(nodes.begin(), nodes.end(), node);

					// compute distance as count of elements
					return it - nodes.begin();
				}


				std::string ScorePLoopIterationStrategy::regionName(SgNode* node) {

					SgFunctionDeclaration* fnDecl = SageInterface::getEnclosingFunctionDeclaration(node);

					std::string name = fnDecl->get_qualified_name();
					std::stringstream postfix;

					postfix << "-";

					switch (node->variantT()) {

					case V_SgForStatement:
					case V_SgWhileStmt:
					case V_SgDoWhileStmt:
						postfix << loopName(node) << loopNodeIndex(node) << "-iteration";
						break;

					default:
						name = regionName(node->get_parent());
						postfix << "?-iteration";
						break;
					}

					return name + postfix.str();
				}


				std::string ScorePLoopIterationStrategy::regionHandleName(SgNode* node) {

					std::map<SgNode*, std::string>::iterator i = nameHistory.find(node);

					if (i != nameHistory.end()) {
						return i->second;
					}

					std::string handleName = SageInterface::generateUniqueVariableName(SageInterface::getScope(node), "InstROHandle");

					nameHistory[node] = handleName;

					return handleName;
				}


				std::vector<SgDeclarationStatement*> ScorePLoopIterationStrategy::constructDeclarationInMainFileFor(SgNode* node) {

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


				std::vector<SgDeclarationStatement*> ScorePLoopIterationStrategy::constructDeclarationInLocalFileFor(SgNode* node) {

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


				std::vector<SgStatement*> ScorePLoopIterationStrategy::constructInitializationInMainScopeFor(SgNode* node) {

					std::vector<SgStatement*> result;

					std::string name = regionName(node);

					std::string handle = regionHandleName(node);

					std::string regionTypeName = "SCOREP_USER_REGION_TYPE_LOOP";

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


				std::vector<SgStatement*> ScorePLoopIterationStrategy::constructEntryInstrumentationInsideNthScopeFor(SgScopeStatement* node, size_t index) {

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


				std::vector<SgStatement*> ScorePLoopIterationStrategy::constructExitInstrumentationInsideNthScopeFor(SgScopeStatement* node, size_t index) {

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


