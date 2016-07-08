#include "omp.h"

int main(int argc, char** argv) {
	int a;
#pragma omp parallel
	{
#pragma omp master
		a = 42;

#pragma omp for nowait
		for (int i = 0; i < 100; ++i) {
#pragma omp critical
			a += i;
		}
		int k = 0;
#pragma omp for
		for (int i = 40; i > 0; --i) {
			k += a / i;
		}
	}

	return 0;
}
