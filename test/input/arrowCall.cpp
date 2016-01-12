struct A {
	void b() {}
};

void foo() {
	A a;
	A *pa = &a;
	pa->b();
}
