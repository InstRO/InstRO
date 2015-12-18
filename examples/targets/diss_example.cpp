int example(int arg1, float arg2) {
	int a = arg1;
	if (arg1 * arg2 < 100) {
		a *= 10;
		example(a, arg2);
	}
	for (int i = 0; i < arg1; i++) {
		a *= a;
	}
	return a;
}

int main(int argc, char** argv) {
	example(10, 100.0);
	return 0;
}

