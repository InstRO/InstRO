

template<int n1, int n2>
bool eq(){
	return n1 == n2;
}

void test(){
	bool b = eq<2, 2>();
}

int main(int argc, char **){
	test();
	return 0;
}
