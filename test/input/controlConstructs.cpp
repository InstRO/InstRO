
void leaf() {}

int ifFunction(int a, int b) {
	if (a > b) {
		leaf();
	}
	for (int i = 0; i < a; ++i) {
		b--;
	}
	return b;
}

int main(int argc, char **argv) {
	if (int a = ifFunction(4, 8) > 4) {
		do {
			a--;
		} while (a > 0);
	}

	return 0;
}
