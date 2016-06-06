#ifndef INSTRO_CORE_OPTIONS_H
#define INSTRO_CORE_OPTIONS_H

// Brings in the macro list of options
#include "instro/core/OptionDefs.minc"
#include "instro/utility/Iterator.h"

namespace InstRO {
namespace Core {
/**
 * Structure which holds defines for the command line options.
 * This structure should be used when referring to the mapping between an
 * option and its spelling on the command line.
 *
 * XXX This should have a more expressive name.
 */
struct Options {
	typedef std::string value_type;
#define REGISTER_FIRST_OPTION(OptName, CmdName, DType, Desc) \
	value_type OptName = #CmdName; \
	OPTIONS_ITERATOR(begin, OptName)

#define REGISTER_OPTION(OptName, CmdName, DType, Desc) value_type OptName = #CmdName;

#define REGISTER_LAST_OPTION(OptName, CmdName, DType, Desc) \
	value_type OptName = #CmdName; \
	OPTIONS_ITERATOR(end, ESCAPE(OptName + 1))

	REGISTER_OPTION_LIST

#undef REGISTER_FIRST_OPTION
#undef REGISTER_OPTION
#undef REGISTER_LAST_OPTION
};

/**
 * Structure for the actual runtime values of the possible arguments.
 * 
 * Are accessed like:
 * OptionArguments args = handler.apply();
 * auto value = args.OptName;
 *
 */
struct OptionArguments {
#define REGISTER_FIRST_OPTION(OptName, CmdName, DType, Desc) DType OptName;
#define REGISTER_OPTION(OptName, CmdName, DType, Desc) DType OptName;
#define REGISTER_LAST_OPTION(OptName, CmdName, DType, Desc) DType OptName;

	REGISTER_OPTION_LIST

#undef REGISTER_FIRST_OPTION
#undef REGISTER_OPTION
#undef REGISTER_LAST_OPTION
};

#undef REGISTER_OPTION_LIST
}	// Core
}	// InstRO
#endif
