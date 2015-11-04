#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Selector {

/** \brief Get the main function */
class ProgramEntrySelector : public InstRO::Core::PassImplementation {

 public:
	ProgramEntrySelector() : PassImplementation(Core::ChannelConfiguration()){};
	virtual void init() override {}
	virtual void execute() override;
	virtual void finalize() override {}

};

}	// namespace Selector
}	// namespace InstRO
