int foo(int k){
	int res = 12 / 2;
	if(k > res) {
		return res + k;
	} else {
		int x = res - k;
		{
			return x;
		}
	}
}

int main(int argc, char **argv){
	int val = -1;
	for(int i = 0; i < 20; ++i){
		val = foo(i);
		if(val < 5) {
			val--;
		}
	}
	return (val > 20) ? 0 : 1;
}
