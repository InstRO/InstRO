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
class GCCLikeCommandLinePreparationStrategy {
 public:
	GCCLikeCommandLinePreparationStrategy(int *argc, char ***argv)
			: argcP(argc),
				argvP(argv),
				desc("InstRO options"),
				instroIncludeOptionName("instro-include"),
				instroLibPathOptionName("instro-library-path"),
				instroLibNameOptionName("instro-library-name") {

		desc.add_options()(instroIncludeOptionName.c_str(), bpo::value<std::string>(),
											 "sets where InstRO finds its supporting includes")(
				instroLibPathOptionName.c_str(), bpo::value<std::string>(), "where InstRO finds the measurement library")(
				instroLibNameOptionName.c_str(), bpo::value<std::string>(),
				"the name of the shared library to link (without the preceding \"lib\")");

		bpo::store(bpo::command_line_parser(*argcP, *argvP).options(desc).allow_unregistered().run(), vm);
		bpo::notify(vm);

		if (vm.count(instroIncludeOptionName)) {
			logIt(DEBUG) << "InstRO Include set to: " << vm[instroIncludeOptionName].as<std::string>() << std::endl;
			instroIncludePathOption = vm[instroIncludeOptionName].as<std::string>();
		} else {
			instroIncludePathOption = getInstroRTIncludePathname();
		}

		if (vm.count(instroLibPathOptionName)) {
			logIt(DEBUG) << "InstRO Library path set to: " << vm[instroLibPathOptionName].as<std::string>() << std::endl;
			instroLibPathOption = vm[instroLibPathOptionName].as<std::string>();
		} else {
			instroLibPathOption = getInstroRTLibraryPathname();
		}

		if (vm.count(instroLibNameOptionName)) {
			logIt(DEBUG) << "InstRO Library name set to: " << vm[instroLibNameOptionName].as<std::string>() << std::endl;
			instroLibNameOption = vm[instroLibNameOptionName].as<std::string>();
		} else {
			instroLibNameOption = "imi";
		}
	}

	/** strips the instro command line arguments and generates an invocation line that can be passed to Rose */
	std::vector<std::string> getCommandLine() {
		std::vector<std::string> argVec(*argcP);
		for (int i = 0; i < argVec.size(); ++i) {
			argVec[i] = (*argvP)[i];
		}

		// Find and remove the instro specific command line arguments
		argVec.erase(std::remove_if(argVec.begin(), argVec.end(), [&](const std::string &arg) {
									 return ((arg.find(instroIncludeOptionName) != std::string::npos) ||
													 (arg.find(instroLibPathOptionName) != std::string::npos) ||
													 (arg.find(instroLibNameOptionName) != std::string::npos));
								 }), argVec.end());

		auto pos = argVec.begin();
		std::advance(pos, 1);
		argVec.insert(pos, std::string("-I" + getInstroIncludePath()));

		argVec.push_back(std::string("-L" + getInstroLibPath()));

		std::string library = std::string("-l" + getInstroLibName());
		argVec.push_back(library);

		for (auto arg : argVec) {
			logIt(ERROR) << arg << " ";
		}

		return argVec;
	}

	std::string getInstroLibPath() { return instroLibPathOption; }

	std::string getInstroLibName() { return instroLibNameOption; }

	std::string getInstroIncludePath() { return instroIncludePathOption; }

 private:
	int *argcP;
	char ***argvP;
	bpo::variables_map vm;
	bpo::options_description desc;

	const std::string instroIncludeOptionName;
	const std::string instroLibPathOptionName;
	const std::string instroLibNameOptionName;

	std::string instroIncludePathOption;
	std::string instroLibPathOption;
	std::string instroLibNameOption;
};
}
}
#endif
