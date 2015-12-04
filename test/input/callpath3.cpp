void foo(){}

void bar(){
	foo();
}

int main(int argc, char **argv){

	if(true){
		bar();
	}

	return 0;
}
