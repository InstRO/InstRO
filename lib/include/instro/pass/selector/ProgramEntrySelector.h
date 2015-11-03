#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Selector {

class ProgramEntrySelector : public InstRO::Core::PassImplementation {

 public:
	ProgramEntrySelector() : PassImplementation(Core::ChannelConfiguration()){};
	virtual void init() override;
	virtual void execute() override;
	virtual void finalize() override;

 protected:
};
}
}
