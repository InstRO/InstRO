#ifndef INSTRO_UTILITY_ENVIRONMENT_H
#define INSTRO_UTILITY_ENVIRONMENT_H

#include "instro/config.h"

#include <string>

#include "boost/filesystem.hpp"

namespace InstRO {
namespace Utility {

/* Reads the content of the passed environment variable and returns it as a string. */
std::string getEnvironmentVariable(std::string envVar) {
	char *ev = getenv(envVar.c_str());
	if (ev != nullptr) {
		return std::string(ev);
	} else {
		throw std::string("The environment variable " + envVar + " was empty");
	}
}

/** Returns the configure-time installation directory of the instro library */
std::string getInstroInstallationPathname();
/** Returns the configure-time runtime library path (either prefix or build directory) */
std::string getInstroRTLibraryPathname();
/** Returns the configure-time include path (either prefix or source directory) */
std::string getInstroRTIncludePathname();
}
}
#endif
