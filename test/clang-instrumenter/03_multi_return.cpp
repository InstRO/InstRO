

int foo(int x) {
	if (x < 100) {
		return x * x;
	} else if (x > 1000){
		return foo(x / 2);
	}
	return foo(x-1);
}