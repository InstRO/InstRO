// RUN: %instrument %s | FileCheck %s

// CHECK-NOT: Instrumentation failed

// CHECK: int id(int x) {
// CHECK-NEXT: __cyg_profile_func_enter((void*) id, 0);
// CHECK: __cyg_profile_func_exit((void*) id, 0);
int id(int x) {
		return x;
}

// CHECK: int square(int x) {
// CHECK-NEXT: __cyg_profile_func_enter((void*) square, 0);
// CHECK-NEXT: int __instro_{{.*}} = x * x;
// CHECK-NEXT: __cyg_profile_func_exit((void*) square, 0);
// CHECK-NEXT: return  __instro_{{.*}}
int square(int x) {
	return x * x;
}
