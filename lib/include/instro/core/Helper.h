#ifndef INSTRO_STD_HELPER
#define INSTRO_STD_HELPER
namespace std {

#ifdef PROVIDE_MAKE_UNIQUE
// template<typename T, typename... Args> std::unique_ptr<T> make_unique(Args&&... args);
template<typename T, typename... Args> std::unique_ptr<T> make_unique(Args&&... args) {return std::unique_ptr<T>(new T(std::forward<Args>(args)...));}

#endif
}
#endif
