#ifndef INSTRO_UTILITY_EXCEPTION
#define INSTRO_UTILITY_EXCEPTION

#include "instro/config.h"

#include <string>
#include <iostream>
#include <cstdlib>

namespace InstRO {

	/// If exceptions are enabled, throws a std::string with the specified message.
	/// Otherwise the message is fed to standard error and the program aborted.
	inline void raise_exception(const char* msg) {
		#if INSTRO_DISABLE_EXCEPTIONS
			std::cerr << msg << std::endl;
			std::abort();
		#else
			throw std::string(msg);
		#endif
	}

	inline void raise_exception(const std::string &msg) {
		raise_exception(msg.c_str());
	}
}

#endif
