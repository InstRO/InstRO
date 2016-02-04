void foo(){}

void forever(){
	while(false){
		foo();
	}
}

int main(int argc, char **argv){
	forever();
	return 0;
}
