
#include <cstdio>

int fibRecursive(int num){
	if(num == 0) return 0;
	if(num == 1) return 1;

	return fibRecursive(num-1) + fibRecursive(num-2);
}

int fibIterative(int num){
	int f0 = 0;
	int f1 = 1;
	int fib = 0;
	for(int i = 1; i < num; ++i){
		fib = f1 + f0;
		f0 = f1;
		f1 = fib;
	}
	return fib;
}

int main(int argc, char **argv){

	int fib6 = fibRecursive(6);

	int fib6_ = fibIterative(6);

	printf("Fib(6) = %i\n", fib6);
	printf("Fib(6) = %i\n", fib6_);

	return 0;
}
