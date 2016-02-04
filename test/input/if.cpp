void doA() {}
void foo() {
	if (42 > 21) {
		doA();
	}
}

int main(int argc, char **){
	foo();
	return 0;
}
