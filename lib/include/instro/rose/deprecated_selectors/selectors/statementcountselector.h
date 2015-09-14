#ifndef STATEMENTCOUNTSELECTOR_H
#define STATEMENTCOUNTSELECTOR_H

#include "../common.h"

namespace InstRO {

/**
 * \brief This selector only selects functions that have more statements than a given threshold
 * \author Roman Neß
 */
class StatementCountSelector : public OutOfOrderSelector {
 public:
	StatementCountSelector(int threshold);
	/* visits every node (therefore all function definitions) */
	void visit(SgNode* start);

 private:
	int threshold;

	/* counts all inenr statements for a list of statements, by ast traversal */
	int countStatementsByTraversal(SgStatementPtrList& statements);
	/* counts all inenr statements for a list of statements, by node query */
	int countStatementsByQuery(SgStatementPtrList& statements);
};
}

#endif	// STATEMENTCOUNTSELECTOR_H
