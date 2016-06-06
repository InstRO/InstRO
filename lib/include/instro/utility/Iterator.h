#ifndef INSTRO_UTILITY_ITERATOR_H
#define INSTRO_UTILITY_ITERATOR_H

#define OPTIONS_ITERATOR(func, start)                \
	Utility::OptionsIterator<value_type> func() {      \
		Utility::OptionsIterator<value_type> it(&start); \
		return it;                                       \
	}

namespace InstRO {
namespace Utility {

/**
 * The OptionsIterator is used to iterate the struct members of the InstRO command line options.
 * To see how it is used, see i.e. core/Options.h and utilty/CommandlineHandling.cpp
 */
template <typename T>
class OptionsIterator {
	T* data;
	int pos = 0;

 public:
	OptionsIterator(T* d) : data(d) {}
	T operator*() { return static_cast<T>(*(data + pos)); }
	OptionsIterator<T> operator++() {
		pos++;
		return *this;
	}
	bool operator!=(const OptionsIterator<T>& o) { return (data + pos) != (o.data + o.pos); }
};

}	// Utility
}	// InstRO
#endif
