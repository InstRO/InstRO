// RUN: %instrument %s

int id(int x) {
		return x;
}

int square(int x) {
	return x * x;
}
