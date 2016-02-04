struct A {
	void foo();
};

void A::foo(){
	return;
}

int main(int argc, char **){
	A a;
	a.foo();
	return 0;
}
