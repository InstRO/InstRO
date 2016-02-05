int foo() {
	int a = -24;
	float b = -a;
	int c = b + 12;
	return c;
}

int bar() {
	int res = -foo();
	return ~res;
}

int root() {
	int res = +bar();
	return --res;
}

int main(int arc, char **argv) { 
	int a = (-(-(42)));
	float f = - (3 + 2.0f);
	return 0; 
}
