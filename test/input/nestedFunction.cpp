
int main(int argc, char **argv){
struct A {
	int absolut(int a) {
		if(a < 0) {
			return -1 * a;
		} else {
			return a;
		}
	}
};

	int num = -5;
	A a;
	num = a.absolut(num);
	return 0;
}
