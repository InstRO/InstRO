#include "instro/utility/CommandlineHandling.h"

using namespace InstRO;

Utility::RoseCLIPreparation::RoseCLIPreparation(int* argc, char*** argv)
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

std::vector<std::string> Utility::RoseCLIPreparation::getCommandLine() {
	std::vector<std::string> argVec(*argcP);
	for (int i = 0; i < argVec.size(); ++i) {
		argVec[i] = (*argvP)[i];
	}

	// Find and remove the instro specific command line arguments
	argVec.erase(std::remove_if(argVec.begin(), argVec.end(), [&](const std::string& arg) {
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
		logIt(DEBUG) << arg << " ";
	}

	return argVec;
}

