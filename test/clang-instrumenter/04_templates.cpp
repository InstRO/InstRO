// RUN: %instrument %s | FileCheck %s

// CHECK-NOT: Instrumentation failed

// CHECK: int add(int y) {
// CHECK-NEXT: __cyg_profile_func_enter((void*) add, 0);
// CHECK: __cyg_profile_func_exit((void*) add, 0);
template<int x>
int add(int y) {
		return x + y;
}

// CHECK: int foo(T t) {
// CHECK-NEXT:  __cyg_profile_func_enter((void*) foo, 0);
// CHECK-NEXT: 	decltype(t.foo()) __instro_{{.*}} = t.foo();
// CHECK-NEXT: __cyg_profile_func_exit((void*) foo, 0);
// CHECK-NEXT:  return  __instro_{{.*}};
template<typename T>
int foo(T t) {
	return t.foo();
}

