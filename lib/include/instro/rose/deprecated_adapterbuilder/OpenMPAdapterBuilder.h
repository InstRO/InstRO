#ifndef INSTRUMENTOR_OPENMP_ADAPTER__
#define INSTRUMENTOR_OPENMP_ADAPTER__

#include "../common.h"

namespace InstRO {

/**
 * Handles all selected OMP-Statements
 *
 *
 * FIXME 2013-10-08 JP: Implement this adapter builder!
 * \ingroup Adapterbuilder
 * \author InstRO
 */
class OMP_AdapterBuilder : public AdapterBuilder {
 public:
	void buildForNode(SgNode* node, Selector* decidingSelector);

	/* Handlers for nodes with respect to the prepended openmp-directive
					At least one of these should be implemented in order for the adapterbuilder to work */
	virtual void handleOpenMP(SgBasicBlock* node, SgOmpParallelStatement* ompStatement){};
	virtual void handleOpenMP(SgForStatement* node, SgOmpForStatement* ompStatement){};
	virtual void handleOpenMP(SgBasicBlock* node, SgOmpSectionsStatement* ompStatement){};
	virtual void handleOpenMP(SgBasicBlock* node, SgOmpSectionStatement* ompStatement){};
	virtual void handleOpenMP(SgBasicBlock* node, SgOmpSingleStatement* ompStatement){};
	virtual void handleOpenMP(SgBasicBlock* node, SgOmpTaskStatement* ompStatement){};
	virtual void handleOpenMP(SgBasicBlock* node, SgOmpMasterStatement* ompStatement){};
	virtual void handleOpenMP(SgBasicBlock* node, SgOmpCriticalStatement* ompStatement){};
	virtual void handleOpenMP(SgBasicBlock* node, SgOmpOrderedStatement* ompStatement){};
	virtual void handleOpenMP(SgExprStatement* node, SgOmpAtomicStatement* ompStatement){};
	virtual void handleOpenMP(SgOmpBarrierStatement* ompStatement){};
	virtual void handleOpenMP(SgOmpTaskwaitStatement* ompStatement){};
	virtual void handleOpenMP(SgOmpFlushStatement* ompStatement){};
};
}
#endif
