#include "omp.h"
void foo() {}

int main(int argc, char** argv) {
#pragma omp parallel
	{
		for (int i = 0; i < 100; ++i) {
#pragma omp task
			foo();
		}
	}
	return 0;
}
