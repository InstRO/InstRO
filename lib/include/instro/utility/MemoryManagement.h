#ifndef INSTRO_UTILITY_MEMORY_MANAGEMENT_H
#define INSTRO_UTILITY_MEMORY_MANAGEMENT_H

#include <memory>

namespace std {

#ifdef PROVIDE_MAKE_UNIQUE
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif
}
#endif
