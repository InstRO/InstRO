void foo(){}

void forever(){
	while(true){
		foo();
	}
}

int main(int argc, char **argv){
	forever();
	return 0;
}
