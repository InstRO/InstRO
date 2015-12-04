void caseA() {}

void caseB() {}

int main(int argc, char **argv) {
	switch (argc) {
		case 1:
			caseA();
			break;
		case 2:
			caseB();
			break;
		default:
			return -1;
	}

	return 0;
}
