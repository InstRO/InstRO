
void calculate(double d) {}

template <typename _T>
void foo(_T t) {
	calculate(t);
}

void bar() {
	foo(4.23);

	foo(4);
}
int main(int argc, char **){
	return 0;
}
