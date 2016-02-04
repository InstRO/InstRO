void foo(){}

void forever(){
	while(true){
		foo();
	}
}

int main(int argc, char **argv){
	foo();	
	return 0;
}
