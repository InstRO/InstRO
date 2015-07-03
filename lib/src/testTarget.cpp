int foo(int depth) {
	if (depth == 0)
		return 1;
	return depth * foo(depth - 1);
}

int myGoo() { return 10; }

int main