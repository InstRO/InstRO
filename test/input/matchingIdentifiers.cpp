int foo() { return 1; }

int bar() { return 2; }

int wrap() { return 43; }

int yaf() { return 2; }

int main(int argc, char** argv) {
	int res = foo() + bar() + yaf();
	int k = bar() + 24;
	return 0;
}
