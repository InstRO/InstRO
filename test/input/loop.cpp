
void doSomething() {}

void foo() {
	for (int i = 0; i < 100; ++i) {
		doSomething();
	}
}

int main(int argc, char **){
	foo();
	return 0;
}
