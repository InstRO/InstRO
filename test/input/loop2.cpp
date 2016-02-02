void doSomething(){}

void foo(){
	int i = 0;
	for(; i < 100; ++i){	// RN: do we allow empty statements here?
		doSomething();
	}
}

int main(int argc, char **){
	foo();
	return 0;
}
