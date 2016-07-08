#include "omp.h"

int main(int argc, char** argv) {
	int i = 0;
#pragma omp parallel
	{
#pragma omp atomic
		i++;
	}
	return 0;
}
