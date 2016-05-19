struct A {};

struct MyClass {
 public:
	MyClass() : x(0) {}
	~MyClass() {}

 private:
	int x;
};

int main(int argc, char **argv) {
	{
		MyClass mc;
		A a;
	}
	return 0;
}
