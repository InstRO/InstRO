#include <memory>
#include <set>
#include <iostream>
#include "instro/core/ConstructSet.h"
#include "instro/rose/core/RoseConstructSet.h"

#ifdef HAVE_ROSE
#include "rose.h"
#else
#endif

struct InstrumentableConstructPredicate {
	bool operator()(SgNode* n) const {
		if (isSgDoWhileStmt(n) || isSgBasicBlock(n) || isSgFunctionDefinition(n))
			return true;
		if (isSgExpression(n) != nullptr)
			return true;
		return false;
	}
};
