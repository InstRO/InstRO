#ifndef INSTRO_UTILITY_EXCEPTION
#define INSTRO_UTILITY_EXCEPTION

#include <string>
#include <iostream>
#include <cstdlib>

namespace InstRO {
	
	inline void raise_exception(const char* msg) {
		#if 1
			throw std::string(msg);
		#else
			std::cerr << msg << std::endl;
			std::abort();
		#endif
	}
	
	inline void raise_exception(const std::string &msg) {
		raise_exception(msg.c_str());
	}
}

#endif