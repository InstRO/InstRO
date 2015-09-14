
#ifndef CUBEEDGEBASEDSELECTOR_H_
#define CUBEEDGEBASEDSELECTOR_H_

#include "../common.h"

#include <string>

namespace InstRO {

/**
 * \brief Selects nodes based on a metric of the CubeCallGraph tool.
 * \author Roman Neß
 */
class CubeEdgeBasedSelector : public OutOfOrderSelector {
 public:
	CubeEdgeBasedSelector(SgProject* project, std::string cubePath);

	void visit(SgNode* n);
};
}

#endif
