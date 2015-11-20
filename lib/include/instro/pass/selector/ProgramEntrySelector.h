#ifndef INSTRO_PASS_SELECTOR_PROGRAM_ENTRY_SELECTOR_H
#define INSTRO_PASS_SELECTOR_PROGRAM_ENTRY_SELECTOR_H


#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Selector {

/** \brief Get the main function */
class ProgramEntrySelector : public InstRO::Core::PassImplementation {

 public:
	ProgramEntrySelector() : PassImplementation(Core::ChannelConfiguration()){};
	virtual void execute() override;

};

}	// namespace Selector
}	// namespace InstRO

#endif
