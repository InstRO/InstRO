#ifndef INSTRO_PASS_SELECTOR_CALLPATHSELECTOR_H
#define INSTRO_PASS_SELECTOR_CALLPATHSELECTOR_H

#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Selector {

/**
 * \brief Selects all Constructs on the path between "from" and "to" on the ExtendedCallgraph
 * \author Roman Ness
 * */
class CallPathSelector: public InstRO::Core::PassImplementation {
public:
	CallPathSelector(std::string dotName) :
			InstRO::Core::PassImplementation(),
			dotName(dotName) {}

	virtual void execute() override;

 private:
	std::string dotName;

};

}	// namespace Selector
}	// namespace InstRO

#endif	// INSTRO_PASS_SELECTOR_CALLPATHSELECTOR_H
