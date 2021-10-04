// RUN: %instrument %s | FileCheck %s

// CHECK-NOT: Instrumentation failed

// Note: This is based on the functional_hash.h header from the standard library.

template<typename T>
struct A {
	T foo() {
		return T();
	}
};


template<typename T>
struct S : A<T>
{ };


// Explicit specializations for integer types.
#define define_trivial(T) 	\
  template<>						\
    struct S<T>: A<T>  \
    {                                                   \
      unsigned long                                            \
      operator()(T t) const noexcept              \
      { return static_cast<unsigned long>(t); }            \
    };

/// Explicit specialization for bool.
define_trivial(bool)

/// Explicit specialization for char.
define_trivial(char)