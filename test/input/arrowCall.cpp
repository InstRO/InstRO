struct A {
	void b() {}
};

void foo() {
	A a;
	A *pa = &a;
	pa->b();
}

int main(int argc, char **){
	return 0;
}
