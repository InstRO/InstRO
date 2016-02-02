void foo() {
	while (int a = 42) {
		a++;
		if (a > 100)
			break;
	}
}

int main(int argc, char **){
	foo();
	return 0;
}
