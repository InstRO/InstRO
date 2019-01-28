#ifndef INSTRO_UTILITY_COMMANDLINE_HANDLING_H
#define INSTRO_UTILITY_COMMANDLINE_HANDLING_H

#include "boost/program_options/options_description.hpp"
#include "boost/program_options/parsers.hpp"
#include "boost/program_options/variables_map.hpp"

#include "instro/utility/Environment.h"
#include "instro/utility/Logger.h"

#include "instro/core/Options.h"

namespace InstRO {
namespace Utility {
namespace bpo = boost::program_options;

/**
 * Commandline Handler for the basic InstRO command line options.
 * Runs the parser and returns an OptionArguments struct which holds the
 * actually supplied argument values.
 */
class CommandLineHandler {
 public:
	// This would then be required for all handlers.
	typedef InstRO::Core::OptionArguments OptionArguments;

	explicit CommandLineHandler(int argc, char** argv);
	CommandLineHandler() = delete;

	/** Runs the command line parser and build the OptionsArguments struct */
	OptionArguments apply();

	/** If not yet run, runs the parser and returns the OptionsArguments struct */
	OptionArguments getArguments();

 private:
	bpo::command_line_parser cmdParser;
	bpo::options_description desc;
	bpo::variables_map vm;
	OptionArguments args;
	bool hasRun;
};

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

	CommandLineHandler clh;

	std::string instroIncludePathOption;
	std::string instroLibPathOption;
	std::string instroLibNameOption;
};

}	// namespace Utility
}	// namespace InstRO
#endif
