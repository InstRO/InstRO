#include "instro/utility/Environment.h"

#include "instro/config.h"

std::string InstRO::Utility::getEnvironmentVariable(std::string envVar) {
	char *ev = getenv(envVar.c_str());
	if (ev != nullptr) {
		return std::string(ev);
	} else {
		throw std::string("The environment variable " + envVar + " was empty");
	}
}

std::string InstRO::Utility::getInstroRTLibraryPathname() {
	InstRO::Config::BuildInstallConfig cfg;
	std::string pap(cfg.prefixAbsolutePath);
	if (pap != "NONE") {
		// TODO Probe whether it is installed already?
		return pap + "/lib";
	}

	std::string brp(cfg.builddirRealpath);
	if (!brp.empty()) {
		return brp + "/support/.libs";
	}

	throw std::string("InstRO was not able to determine reasonable default paths");
}

std::string InstRO::Utility::getInstroRTIncludePathname() {
	InstRO::Config::BuildInstallConfig cfg;
	std::string pap(cfg.prefixAbsolutePath);
	if (pap != "NONE") {
		return pap + "/include";
	}

	std::string brp(cfg.repositoryRealpath);
	if (!brp.empty()) {
		return brp + "/../support";
	}

	throw std::string("InstRO was not able to determine reasonable default paths");
}

std::string InstRO::Utility::getInstroInstallationPathname() {
	InstRO::Config::BuildInstallConfig cfg;
	if (std::string(cfg.prefixAbsolutePath) != "NONE") {
		// TODO Probe whether it is installed already?
		return std::string(cfg.prefixAbsolutePath);
	}

	if (!std::string(cfg.builddirRealpath).empty()) {
		return std::string(cfg.builddirRealpath);
	}

	throw std::string("InstRO was not able to determine reasonable default paths");
}


std::string InstRO::Utility::getScorepIncludeFlags() {
	auto version = runExecutableAndReturnResult(std::string("/bin/sh"), std::string("-c"),
																							std::string("scorep-config --version > .instro_tmp_out"));

	std::string cxxFlags("--cxxflags");	// for scorep 1.4.2
	if (version.compare("1.2.2") == 0) {
		cxxFlags = std::string(" --cppflags");
	}

	auto s = runExecutableAndReturnResult(std::string("/bin/sh"), std::string("-c"),
																				std::string("scorep-config " + cxxFlags + " > .instro_tmp_out"));
	return s;
}

void InstRO::Utility::printErrorAndExit(int errNo) {
	logIt(ERROR) << "Running sub program. Msg: " << std::string(strerror(errno)) << std::endl;
	exit(-1);
}

void InstRO::Utility::exitNoError() { exit(0); }
