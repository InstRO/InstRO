#ifndef INSTRO_UTILITY_EXCEPTION_H
#define INSTRO_UTILITY_EXCEPTION_H

#include "instro/config.h"

#include <string>
#include <iostream>
#include <cstdlib>

namespace InstRO {

/// If exceptions are enabled, throws a std::string with the specified message.
/// Otherwise the message is fed to standard error and the program aborted.
#if INSTRO_USE_ATTRIBUTE_NORETURN
	__attribute__ ((noreturn)) inline void raise_exception(const char* msg) {
#else
	inline void raise_exception(const char* msg) {
#endif
		
#if INSTRO_DISABLE_EXCEPTIONS
	std::cerr << msg << std::endl;
	std::abort();
#else
	throw std::string(msg);
#endif
}

#if INSTRO_USE_ATTRIBUTE_NORETURN
	__attribute__ ((noreturn)) inline void raise_exception(const std::string& msg) {
#else
	inline void raise_exception(const std::string& msg) {
#endif
 raise_exception(msg.c_str()); }
}

#endif
