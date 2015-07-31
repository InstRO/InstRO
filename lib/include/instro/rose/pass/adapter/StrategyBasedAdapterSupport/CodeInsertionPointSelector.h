#ifndef CODE_INSERTION_POINT_SELECTOR_H__
#define CODE_INSERTION_POINT_SELECTOR_H__

#include <rose.h>

#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/CodeInsertionHelper.h"

/**
 * Functions for various code insertion strategies
 * 
 * \author Matthias Zoellner
 */

namespace InstRO {
	namespace Rose {
		namespace Adapter {
			namespace StrategyBasedAdapterSupport{

				/**
				 * Returns a CodeInsertionHelper object for the global scope in front of the projects main function
				 * Statement insertion is restricted to SgDeclarationStatement and derived classes
				 */
				CodeInsertionHelper getMainGlobalDeclarationPoint(SgNode* node);

				/**
				 * Returns a CodeInsertionHelper object to insert statements at the entry point of main function.
				 *
				 * The entry point is guaranteed to come before any *InitializationPoint, so it may be used to
				 * initialize some tool-wide data.
				 */
				CodeInsertionHelper getMainSetupPoint(SgNode* node);

				/**
				 * Returns a CodeInsertionHelper object for the start of main function
				 * This guarantees to insert to the front of main at point of construction, nothing more
				 */
				CodeInsertionHelper getMainInitializationPoint(SgNode* node);

				/**
				 * Returns a CodeInsertionHelper object for the global scope directly in front of the nodes function
				 * Statement insertion is restricted to SgDeclarationStatement and derived classes
				 */
				CodeInsertionHelper getNearestGlobalDeclarationPoint(SgNode* node);

				/**
				 * Returns a CodeInsertionHelper object for the global scope in front of the nodes function
				 * and as far to the top of the source file as possible. Namely, this may insert in front of
				 * header includes, so it is not allowed to rely on any header based definition.
				 * Statement insertion is restricted to SgDeclarationStatement and derived classes
				 */
				CodeInsertionHelper getEarliestGlobalDeclarationPoint(SgNode* node);

				/**
				 * Returns a CodeInsertionHelper object for the start of the current function
				 * This guarantees to insert to the front of the function at point of construction, nothing more
				 */
				CodeInsertionHelper getFunctionInitializationPoint(SgNode* node);

				/**
				 * Returns a CodeInsertionHelper object to insert statements in front of node
				 *
				 * Not allowed on return statements, since it breaks code order for tool shutdown
				 */
				CodeInsertionHelper getStatementEntryPoint(SgStatement* node);

				/**
				 * Returns a CodeInsertionHelper object to insert statements at each exit point
				 * To archieve this, the statement-to-insert is deep copied for each exit point
				 *
				 * If node is inside a scope where break and continue are effective, corresponding points
				 * are considered
				 *
				 * For C++, the AST is transformed to embed node in a try/catch clause
				 *
				 * Not allowed on return statements, since an instruction after return makes no sense
				 */
				CodeInsertionHelper getStatementExitPoints(SgStatement* node);

				/**
				 * Returns a CodeInsertionHelper object to insert statements in the inner scope of node
				 * This requires node to be a statement with inner scope (like if-else or loops)
				 * For conditional statements there might be multiple entry points (cond-1, cond2 ...). In this
				 * case, the index parameter is used to select a specific entry point
				 */
				CodeInsertionHelper getNthInnerScopeEntryPoint(SgStatement* node, size_t index);

				/**
				 * Returns a CodeInsertionHelper object to insert statements at each inner exit point.
				 * For if/else and switch-case statements, the index parameter denotes the inner statement to
				 * be modified: if:0, else:1, case1:0, ... default:after last case.
				 * To archieve this, the statement-to-insert is deep copied for each exit point
				 * It does not include the end of statement itself
				 *
				 * In contrast to getStatementExitPoints, this includes some inner continue and break statements
				 * which may cause execution of the next loop iteration but are exit points to the inner scope
				 *
				 * For C++, the AST is transformed to embed node in a try/catch clause
				 */
				CodeInsertionHelper getNthInnerScopeExitPoints(SgStatement* node, size_t index);

				/**
				 * Returns a CodeInsertionHelper object to insert statements at each inner exit point of main function.
				 *
				 * The exit points are guaranteed to come after the points from getNthInnerScopeExitPoints, so they may be
				 * used to finalize some tool-wide data.
				 */
				CodeInsertionHelper getMainShutdownPoints(SgNode* node);

			}
		}
	}
} // namespace InstRO


#endif // CODE_INSERTION_POINT_SELECTOR_H__

