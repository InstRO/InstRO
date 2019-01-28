#ifndef INSTRO_UTILITY_EXCEPTION_H
#define INSTRO_UTILITY_EXCEPTION_H

#include "instro/config.h"

#include <cstdlib>
#include <iostream>
#include <string>

namespace InstRO {

/// If exceptions are enabled, throws a std::string with the specified message.
/// Otherwise the message is fed to standard error and the program aborted.
__attribute__((noreturn)) inline void raise_exception(const char* msg) {
#if INSTRO_DISABLE_EXCEPTIONS
	std::cerr << msg << std::endl;
	std::abort();
#else
	throw std::string(msg);
#endif
}

__attribute__((noreturn)) inline void raise_exception(const std::string& msg) { raise_exception(msg.c_str()); }
}	// namespace InstRO

#endif
