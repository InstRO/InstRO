#ifndef FLOPCOUNTSELECTOR_H
#define FLOPCOUNTSELECTOR_H

#include "../common.h"
#include <boost/unordered_map.hpp>

namespace InstRO {

/**
 * \brief This selector only selects functions that have more floating point operations than a given threshold
 * \author Roman Neß
 */
class FlopCountSelector : public OutOfOrderSelector {
 public:
	FlopCountSelector(int threshold);
	/* visits every node (therefore all function definitions) */
	void visit(SgNode* start);

 private:
	int threshold;
	/* stores information of flops of every method for later recursive/transitive counting */
	boost::unordered_map<SgFunctionDefinition*, int> flopsByFunc;

	/* counts the (non-recursive) amount of floating point operations in a function definition */
	int countByQuery(SgFunctionDefinition* funcDef);

	void selectionEnd(SgProject* project);
};
}

#endif	// FLOPCOUNTSELECTOR_H
