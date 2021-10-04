// RUN: %instrument %s | FileCheck %s

// CHECK-NOT: Instrumentation failed

// CHECK: int foo(int x) {
// CHECK-NEXT: __cyg_profile_func_enter((void*) foo, 0);
// CHECK-NEXT: 	if (x < 100) {
// CHECK-NEXT: 		int __instro_{{.*}} = x * x;
// CHECK-NEXT: __cyg_profile_func_exit((void*) foo, 0);
// CHECK-NEXT:  return  __instro_{{.*}};
// CHECK-NEXT: 	} else if (x > 1000){
// CHECK-NEXT: 		int __instro_{{.*}} = foo(x / 2);
// CHECK-NEXT: __cyg_profile_func_exit((void*) foo, 0);
// CHECK-NEXT:  return  __instro_{{.*}};
// CHECK-NEXT: 	}
// CHECK-NEXT: 	int __instro_{{.*}}= foo(x - 1);
// CHECK-NEXT: __cyg_profile_func_exit((void*) foo, 0);
// CHECK-NEXT:  return  __instro_{{.*}};
int foo(int x) {
	if (x < 100) {
		return x * x;
	} else if (x > 1000){
		return foo(x / 2);
	}
	return foo(x-1);
}