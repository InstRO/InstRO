#include <instro/selectors/statementcountselector.h>
#include <iostream>

using namespace InstRO;

// this is a potentially very verbose option
#define VERBOSE 0

/**
  * \brief Constructor, sets the operational threshold
  * \param threshold the minimum number of contained statements for a function definition to be selected
  */
StatementCountSelector::StatementCountSelector(int threshold) {
	this->threshold = threshold;
}

/**
  * \brief Is called on every node of the AST. Selects function definitions if they contain enough statements
  * \param start A node of the AST
  */
void StatementCountSelector::visit(SgNode* start) {
	SgFunctionDefinition* funcDef = isSgFunctionDefinition(start);

	if(funcDef) {
		SgBasicBlock* startBlock = funcDef->get_body();
		// count statements for current function definition
		int statementCount = countStatementsByTraversal(startBlock->get_statements());

		if(statementCount >= threshold) {
			this->select(funcDef);

#if VERBOSE
			std::cout << funcDef->get_declaration()->get_qualified_name() << " has "
					  << statementCount << " statements." << std::endl << std::endl;
#endif
		}
	}
}

/**
  * \brief Counts the amount of all (inner) statements for a list of starting statements.
  *	Note that some statements may contain a basic block, which contains multiple statements itself.
  * This implementation traverses the AST (or rather its statements) to count nested statements correctly.
  * \param statements A list of statements
  */
int StatementCountSelector::countStatementsByTraversal(SgStatementPtrList& statements) {
	int count=0;

	for(SgStatementPtrList::iterator it=statements.begin(); it!=statements.end(); it++) {
		int oldCount = count;

		// ScopeStatement covers most constructs that have a scope containing multiple statements
		SgScopeStatement* scopeStatement = isSgScopeStatement(*it);
		if(scopeStatement) {
			// count statements IN the current statement recursively
			SgStatementPtrList innerStatements = scopeStatement->generateStatementList();
			count += countStatementsByTraversal(innerStatements);

		// SgCaseOptionStmt is no ScopeStatement
		} else if(isSgCaseOptionStmt(*it)){
			SgBasicBlock* caseBlock = isSgBasicBlock(isSgCaseOptionStmt(*it)->get_body());
			if(caseBlock) {
				count += countStatementsByTraversal(caseBlock->get_statements());
			}

		// SgDefaultOptionStmt is no ScopeStatement
		} else if(isSgDefaultOptionStmt(*it)){
			SgBasicBlock* defaultBlock = isSgBasicBlock(isSgDefaultOptionStmt(*it)->get_body());
			if(defaultBlock) {
				count += countStatementsByTraversal(defaultBlock->get_statements());
			}

		// SgTryStmt is no ScopeStatement
		} else if(isSgTryStmt(*it)){
			SgBasicBlock* tryBlock = isSgBasicBlock(isSgTryStmt(*it)->get_body());
			if(tryBlock) {
				count += countStatementsByTraversal(tryBlock->get_statements());
				// also consider the catch statements
				SgCatchStatementSeq* catchStatements = isSgTryStmt(*it)->get_catch_statement_seq_root();
				count += countStatementsByTraversal(catchStatements->get_catch_statement_seq());
			}

		} //else {
			// count up for simple statements AND statements with scope
			count++;
//		}

#if VERBOSE
		std::cout << "  found "<< count-oldCount << " stmts in:\t"
				  << (*it)->unparseToString() << std::endl;
#endif
	}
	return count;
}

/**
  * \brief Counts the amount of all (inner) statements for a list of starting statements.
  *	Note that some statements may contain a basic block, which contains multiple statements itself.
  * This implementation iterates over all basic blocks of a starting statement, to handle nested statements.
  * \param statements A list of statements
  */
int StatementCountSelector::countStatementsByQuery(SgStatementPtrList& statements) {
	int count = 0;

	// for all statements
	for(SgStatementPtrList::iterator it=statements.begin(); it!=statements.end(); it++) {
		// query all basic blocks in a statement
		std::vector<SgNode*> bBlocks = NodeQuery::querySubTree(*it, V_SgBasicBlock);
		// for all basic blocks in a statement
		for(std::vector<SgNode*>::iterator it2=bBlocks.begin(); it2!=bBlocks.end(); it2++) {
			int localCount = 0;
			SgBasicBlock* basicBlock = isSgBasicBlock(*it2);

			if(basicBlock) {
				// count statements inside each of the basic blocks
				SgStatementPtrList& innerStatements = basicBlock->get_statements();
				localCount += innerStatements.size();
				count += localCount;
#if VERBOSE
				std::cout << "\t" << (*it2)->unparseToString() << " has " << localCount
						  << " statements. Now overall: " << count << std::endl;
#endif
			}
		}
	}

	// also count statements on the highest level
	count += statements.size();

#if VERBOSE
	std::cout << "\tadded " << statements.size() <<
				 " top-level statements. Now overall: " << count << std::endl;
#endif

	return count;
}
