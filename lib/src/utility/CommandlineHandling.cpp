#include "instro/utility/CommandlineHandling.h"

#include "instro/core/OptionDefs.minc"	// Gives definition of register_option_list macro

using namespace InstRO::Utility;

CommandLineHandler::CommandLineHandler(int argc, char** argv)
		: cmdParser(argc, argv), desc("InstRO Base Options"), hasRun(false) {
	namespace bpo = boost::program_options;

	// We do not want to write the same expansion so many times.
#define REGISTER_FIRST_OPTION(OptName, CmdName, DType, Desc) \
	OPTION(REGISTER, ESCAPE(OptName), ESCAPE(CmdName), ESCAPE(DType), ESCAPE(Desc))
#define REGISTER_LAST_OPTION(OptName, CmdName, DType, Desc) \
	OPTION(REGISTER, ESCAPE(OptName), ESCAPE(CmdName), ESCAPE(DType), ESCAPE(Desc))

#define REGISTER_OPTION(OptName, CmdName, DType, Desc) desc.add_options()(#CmdName, bpo::value<DType>(), Desc);

	REGISTER_OPTION_LIST

#undef REGISTER_OPTION

	cmdParser.options(desc).allow_unregistered();
}

CommandLineHandler::OptionArguments CommandLineHandler::apply() {
	namespace bpo = boost::program_options;
	bpo::store(cmdParser.run(), vm);
	boost::program_options::notify(vm);

// We just want to apply this one. So this should be generated.
#define REGISTER_OPTION(OptName, CmdName, DType, Desc) \
	if (vm.count(#CmdName)) {                            \
		args.OptName = vm[#CmdName].as<DType>();           \
	} else {                                             \
		args.OptName = DType();                            \
	}

	REGISTER_OPTION_LIST

#undef REGISTER_OPTION
#undef REGISTER_FIRST_OPTION
#undef REGISTER_LAST_OPTION
#undef REGISTER_OPTION_LIST
	hasRun = true;
	return args;
}

CommandLineHandler::OptionArguments CommandLineHandler::getArguments() {
	if (!hasRun) {
		return apply();
	}
	return args;
}

RoseCLIPreparation::RoseCLIPreparation(int* argc, char*** argv) : argcP(argc), argvP(argv), clh(*argc, *argv) {
	Core::OptionArguments opts = clh.apply();
	if (!opts.instroIncludePath.empty()) {
		instroIncludePathOption = opts.instroIncludePath;
	} else {
		instroIncludePathOption = getInstroRTIncludePathname();
	}

	if (!opts.instroLibraryPath.empty()) {
		instroLibPathOption = opts.instroLibraryPath;
	} else {
		instroLibPathOption = getInstroRTLibraryPathname();
	}

	if (!opts.instroLibraryName.empty()) {
		instroLibNameOption = opts.instroLibraryName;
	} else {
		instroLibNameOption = "imi";
	}

	std::cout << opts.instrumentCtors << std::endl;
}

std::vector<std::string> RoseCLIPreparation::getCommandLine() {
	std::vector<std::string> argVec(*argcP);
	for (int i = 0; i < argVec.size(); ++i) {
		argVec[i] = (*argvP)[i];
	}

	Core::Options optNames;
	// Find and remove the instro specific command line arguments
	argVec.erase(std::remove_if(argVec.begin(), argVec.end(),
															[&](const std::string& arg) {
																for (auto s : optNames) {
																	if (arg.find(s) != std::string::npos) {
																		return true;
																	}
																}
																return false;
															}),
							 argVec.end());

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

