void leaf(){}
void foo(){}

bool onPath(){
	while(true) leaf();
	return true;
}

void alsoOnPath(){
	if(onPath()){
		foo();
	}
}

int main(int argc, char **argv){
	alsoOnPath();
}
