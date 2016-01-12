template<typename _T>
struct container {
	_T data[100];
	int _cur;

	container() : _cur(0) {}

	void insert(_T t){
		data[_cur] = t;
		_cur++;
	}

	_T get(int idx){
		return data[idx];
	}
};

int main(int argc, char **argv){
	container<double> c;
	c.insert(3.41);
	double d = c.get(0);

	return 0;
}
