
void foo(int a){
	int b = a;
}

void foo(double d){
	double b = d;
}

void foo(char c){
	char d = c;
}


int main(int argc, char **){
	foo(4);
	foo(42.0);
	foo('x');
	return 0;
}
