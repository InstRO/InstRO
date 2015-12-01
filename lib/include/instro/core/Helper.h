#ifndef INSTRO_CORE_HELPER_H
#define INSTRO_CORE_HELPER_H
namespace std {

#ifdef PROVIDE_MAKE_UNIQUE
// template<typename T, typename... Args> std::unique_ptr<T> make_unique(Args&&... args);
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif
}
#endif
