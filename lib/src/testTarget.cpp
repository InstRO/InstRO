extern "C" int printf(char * string);
int myFoo();
int myRecursion(int depth);
int myFooBar();

int main(int argc,char ** argv){
	printf("Hello World\n");
	myRecursion(myFoo());
	int variableDeclaration;
	int variableDeclarationAndInitialization;
	return myRecursion(10);
}

int myRecursion(int depth){
	if (depth>0) return myRecursion(depth-1)*depth;
	myFoo();
	return 1;
}

int myFoo()
{
	printf("This is s.th. else\n");
	return 10;
}

int myFooBar(){
	return 100;
}

