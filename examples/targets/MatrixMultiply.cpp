#include <cstdlib>
#include <cstdio>
#include <cmath>

inline void multiplicationKernel(const int i, const int j, const int k, const float *A, const int sizeA, const float *B, const int sizeB, float *C, const int sizeC){
	C[i*sizeC + j] += A[i*sizeA + k] * B[k*sizeB + j];
}

void matrixMultiply(const float *A, const int sizeA, const float *B, const int sizeB, float *C, int sizeC) {
	for(int i = 0; i < sizeA; ++i){
		for(int j = 0; j < sizeB; ++j){
			for(int k = 0; k < sizeC; ++k){
				multiplicationKernel(i, j, k, A, sizeA, B, sizeB, C, sizeC);
			}
		}
	}
}

void initMatrix(float *matrix, const int size){
	for(int i = 0; i < size*size; ++i){
		matrix[i] = i;
	}
}

void print(const float *matrix, const int size){
	for(int i = 0; i < size; ++i){
		printf("\n");
		for(int j = 0; j < size; ++j){
			printf("%f ", matrix[i*size + j]);
		}
	}
	printf("\n");
}

int main(int argc, char **argv){

	const int sizeA = 2;
	const int sizeB = sizeA;
	const int sizeC = sizeB;
	const int allocSizeA = sizeA * sizeA;
	const int allocSizeB = sizeB * sizeB;
	const int allocSizeC = sizeC * sizeC;

	float *matrixA = (float*) malloc(allocSizeA * sizeof(float));
	float *matrixB = (float*) malloc(allocSizeB * sizeof(float));
	float *matrixC = (float*) malloc(allocSizeC * sizeof(float));


	initMatrix(matrixA, sizeA);
	initMatrix(matrixB, sizeB);
	initMatrix(matrixC, sizeC);

	print(matrixA, sizeA);
	printf("\n");
	print(matrixB, sizeB);
	printf("\n");
	print(matrixC, sizeC);

	matrixMultiply(matrixA, sizeA, matrixB, sizeB, matrixC, sizeC);

	print(matrixC, sizeC);

	free(matrixA);
	free(matrixB);
	free(matrixC);

	return 0;
}
