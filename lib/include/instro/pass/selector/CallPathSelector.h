#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Selector {

/**
 * \brief Selects all Constructs on the path between "from" and "to" on the ExtendedCallgraph
 * \author Roman Ness
 * */
class CallPathSelector: public InstRO::Core::PassImplementation {
public:
	CallPathSelector(Pass *from, Pass *to) :
			InstRO::Core::PassImplementation(Core::ChannelConfiguration(from, to)),
			fromCS(from->getOutput()),
			toCS(to->getOutput()) {}

	virtual void execute() override;

 private:
	InstRO::Core::ConstructSet *fromCS, *toCS;

};

}	// namespace Selector
}	// namespace InstRO
