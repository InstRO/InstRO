#ifndef INSTRO_UTILITY_COMMANDLINE_HANDLING_H
#define INSTRO_UTILITY_COMMANDLINE_HANDLING_H

#include "boost/program_options/options_description.hpp"
#include "boost/program_options/variables_map.hpp"
#include "boost/program_options/parsers.hpp"

#include "instro/utility/Logger.h"
#include "instro/utility/Environment.h"

namespace InstRO {
namespace Utility {
namespace bpo = boost::program_options;

/**
 * The class provides functionality which adds the command line options instro-include, instro-library-path and
 * instro-library-name which tell the instrumentor where to look for the supporting headers and where to look for the
 * supporting library (libimi).
 * Does not modify argc, argv (yet)
 */
class RoseCLIPreparation {
 public:
	/** Initializes the members to the respective default values */
	RoseCLIPreparation(int* argc, char*** argv);

	/** strips the instro command line arguments and generates an invocation line that can be passed to Rose */
	std::vector<std::string> getCommandLine();

	/** Returns the instro library path */
	std::string getInstroLibPath() { return instroLibPathOption; }

	/** Returns the instro library name */
	std::string getInstroLibName() { return instroLibNameOption; }

	/** Returns the instro include path */
	std::string getInstroIncludePath() { return instroIncludePathOption; }

 private:
	int* argcP;
	char*** argvP;
	bpo::variables_map vm;
	bpo::options_description desc;

	const std::string instroIncludeOptionName;
	const std::string instroLibPathOptionName;
	const std::string instroLibNameOptionName;

	std::string instroIncludePathOption;
	std::string instroLibPathOption;
	std::string instroLibNameOption;
};

} // Utility
} // InstRO
#endif
