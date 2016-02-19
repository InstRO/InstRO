void foo() {}

int bar(double d) { return (int)d; }

int heavyComp(double a, double b) {
	int mod = bar(a) % bar(b);
	return (a - mod) * (b + mod);
}

double comp(double a, double b) {
	int m = bar(a) << 2;
	double sum = 0;
	for (int i = 0; i < m; ++i) {
		sum += heavyComp(a, b) / m;
	}
	return sum;
}

int main(int argc, char **argv) {
	double res = comp(10, 3);
	return 0;
}
