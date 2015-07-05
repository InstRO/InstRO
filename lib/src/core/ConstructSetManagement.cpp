#include "instro/core/ConstructSetManagement.h"
namespace InstRO {
namespace Core {
ConstructSet* PassConstructSetManagement::getInput(PassConstructSetManagement* from) {
	if (inputOverride.find(from) == inputOverride.end()) {
		return from->getOutput();
	} else {
		return inputOverride[from];
	}
#ifdef __EXCEPTIONS
	throw std::string("I should no be here. It is impossible!!!");
#else
	std::cerr << "I should not be here. It is impossible!" << std::endl;
#endif
}

void PassConstructSetManagement::overrideInput(PassConstructSetManagement* from, ConstructSet* overrideSet) {
	inputOverride[from] = overrideSet;
}
}
}
