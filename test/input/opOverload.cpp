

struct MyInt {
	int v_;
	MyInt operator=(int v) {
		v_ = v;
		return *this;
	}
};

int main(int argc, char **argv) {
	int value = 35;
	MyInt vv;
	vv = value;
	return 0;
}
