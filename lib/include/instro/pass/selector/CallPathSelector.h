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
	CallPathSelector(Pass *from, Pass *to, std::string dotName) :
			InstRO::Core::PassImplementation(Core::ChannelConfiguration(from, to)),
			fromPass(from),
			toPass(to),
			dotName(dotName) {}

	virtual void execute() override;

 private:
	Pass *fromPass, *toPass;
	std::string dotName;

};

}	// namespace Selector
}	// namespace InstRO

#endif	// INSTRO_PASS_SELECTOR_CALLPATHSELECTOR_H
