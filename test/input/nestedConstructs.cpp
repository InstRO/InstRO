

int main(int argc, char **argv){

	for(int i = 0; i < 100; ++i) {
		int j = i % 13;
		do {
			switch (j) {
				case 2: ++j; break;
				case 3: j = 43; break;
				case 5: ++j; continue;
				default: ++j;
			}
		} while (j < 42);
	}

	return 0;
}
