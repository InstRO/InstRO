struct Base {
	int vals[4];

	template <typename _T>
	void foo(_T *t) {
		vals[0] = *t;
	}
};

int main(int argc, char **argv){
	Base b;
	int value = 24;
	b.foo(&value);

	return 0;
}
