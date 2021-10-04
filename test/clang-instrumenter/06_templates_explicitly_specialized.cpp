// RUN: %instrument %s | FileCheck %s

// CHECK-NOT: Instrumentation failed

// CHECK: void foo(T t2) {
// CHECK-NEXT: __asm__("_0Laf:
// CHECK-NEXT: movq %rdi, -8(%rbp)
// CHECK-NEXT: movq 8(%rbp), %rax
// CHECK-NEXT: movq %rax, %rsi
// CHECK-NEXT: movl $_0Laf, %edi
// CHECK-NEXT: call __cyg_profile_func_enter
// CHECK-NEXT: ");
// CHECK-NEXT: this->t = t2;
// CHECK: __asm__("_	movq %rdi, -8(%rbp)
// CHECK-NEXT: movq 8(%rbp), %rax
// CHECK-NEXT: 		movq %rax, %rsi
// CHECK-NEXT: 		movl $_0Laf, %edi
// CHECK-NEXT: 		call __cyg_profile_func_exit
// CHECK-NEXT: ");
template<typename T>
struct S {
	T t;
	void foo(T t2) {
		this->t = t2;
	}
};

// CHECK: void foo(int t2) {
// CHECK-NEXT: __asm__("_1Laf:
// CHECK-NEXT: movq %rdi, -8(%rbp)
// CHECK-NEXT: movq 8(%rbp), %rax
// CHECK-NEXT: movq %rax, %rsi
// CHECK-NEXT: movl $_1Laf, %edi
// CHECK-NEXT: call __cyg_profile_func_enter
// CHECK-NEXT: ");
// CHECK-NEXT: this->t = t2 * 2;
// CHECK: __asm__("_	movq %rdi, -8(%rbp)
// CHECK-NEXT: movq 8(%rbp), %rax
// CHECK-NEXT: 		movq %rax, %rsi
// CHECK-NEXT: 		movl $_1Laf, %edi
// CHECK-NEXT: 		call __cyg_profile_func_exit
// CHECK-NEXT: ");
template<>
struct S<int> {
	int t;
	void foo(int t2) {
		this->t = t2 * 2;
	}
};

// CHECK: void bar() {
// CHECK-NEXT: __cyg_profile_func_enter((void*) bar, 0);
// CHECK-NEXT:	S<int> i;
// CHECK-NEXT:	i.foo(5);
// CHECK: __cyg_profile_func_exit((void*) bar, 0);
void bar() {
	S<int> i;
	i.foo(5);
}
