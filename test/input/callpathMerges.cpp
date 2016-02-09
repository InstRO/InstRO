#include <cstdio>

void markMe(int x) {
	printf("x=%d\n", x);
}

void f10(int x) {
	markMe(2*x);
}

void f11(int x) {
	markMe(3*x);
}

void merge(int x) {
	if (x <= 0)
		f10(x);
	else
		f11(x);		
}

void f00(int x) {
	merge(x*x);
}

void f01(int x) {
	merge(x-42);
}

int main(int argc, char** argv) {
	f00(0);
	f01(1337);

	return 0;
}
