#include <instro/selectors/openmpselector.h>

using namespace InstRO;

void OpenMPSelector::visit(SgNode* node) {
	if (isSgOmpClause(node)) {
		this->select(node);
	} else if (isSgOmpBarrierStatement(node)) {
		this->select(node);
	} else if (isSgOmpBodyStatement(node)) {
		this->select(node);
	} else if (isSgOmpTaskwaitStatement(node)) {
		this->select(node);
	} else if (isSgOmpFlushStatement(node)) {
		this->select(node);
	}
}
