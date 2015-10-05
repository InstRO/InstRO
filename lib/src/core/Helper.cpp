#include <memory>
namespace std {
#ifdef PROVIDE_MAKE_UNIQUE
template <typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif
}
