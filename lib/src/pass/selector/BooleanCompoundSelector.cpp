#include "instro/pass/selector/BooleanCompoundSelector.h"

namespace InstRO {
namespace Selector {

void BooleanCompoundSelector::execute() {

	auto csLeft = getInput(0);
	auto csRight = getInput(1);

	if (operationType == CO_AND) {
		outputSet = csLeft->intersect(*csRight);

	} else if (operationType == CO_OR) {
		outputSet = csLeft->combine(*csRight);

	} else if (operationType == CO_XOR) {
		outputSet = csLeft->symmertricDifference(*csRight);

	} else if (operationType == CO_MINUS) {
		outputSet = csLeft->relativeComplement(*csRight);
	}

}

}	// namespace Selector
}	// namespace InstRO
