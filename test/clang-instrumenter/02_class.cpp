// RUN: %instrument %s | FileCheck %s

// CHECK-NOT: Instrumentation failed


class Test {
// CHECK: int id(int x) {
// CHECK-NEXT: __asm__("_0Laf:
// CHECK-NEXT: movq %rdi, -8(%rbp)
// CHECK-NEXT: movq 8(%rbp), %rax
// CHECK-NEXT: movq %rax, %rsi
// CHECK-NEXT: movl $_0Laf, %edi
// CHECK-NEXT: call __cyg_profile_func_enter
// CHECK-NEXT: ");
// CHECK-NEXT: int __instro_{{.*}} = x;
// CHECK-NEXT: __asm__("_	movq %rdi, -8(%rbp)
// CHECK-NEXT: movq 8(%rbp), %rax
// CHECK-NEXT: 		movq %rax, %rsi
// CHECK-NEXT: 		movl $_0Laf, %edi
// CHECK-NEXT: 		call __cyg_profile_func_exit
// CHECK-NEXT: ");
// CHECK-NEXT: return  __instro_{{.*}};
	int id(int x) {
		return x;
	}

// CHECK: int square(int x) {
// CHECK-NEXT: __asm__("_1Laf:
// CHECK-NEXT: movq %rdi, -8(%rbp)
// CHECK-NEXT: movq 8(%rbp), %rax
// CHECK-NEXT: movq %rax, %rsi
// CHECK-NEXT: movl $_1Laf, %edi
// CHECK-NEXT: call __cyg_profile_func_enter
// CHECK-NEXT: ");
// CHECK-NEXT: int __instro_{{.*}} = x * x;
// CHECK-NEXT: __asm__("_	movq %rdi, -8(%rbp)
// CHECK-NEXT: movq 8(%rbp), %rax
// CHECK-NEXT: 		movq %rax, %rsi
// CHECK-NEXT: 		movl $_1Laf, %edi
// CHECK-NEXT: 		call __cyg_profile_func_exit
// CHECK-NEXT: ");
// CHECK-NEXT: return  __instro_{{.*}};
	int square(int x) {
		return x * x;
	}
};