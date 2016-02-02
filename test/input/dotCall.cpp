struct A {
	void b() {}
};

void foo() {
	A a;
	a.b();
}

int main(int argc, char **){
	foo();
	return 0;
}
