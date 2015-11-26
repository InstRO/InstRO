#ifndef INSTRO_UTILITY_ENVIRONMENT_H
#define INSTRO_UTILITY_ENVIRONMENT_H

#include <string>

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

}
}
#endif
