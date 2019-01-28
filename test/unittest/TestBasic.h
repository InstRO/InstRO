#include <cassert>
#include <iostream>
#include <map>
#include <string>

std::map<std::string, bool> testcases;

void good(std::string testName) { testcases[testName] = true; }

void bad(std::string testName) { testcases[testName] = false; }

#define TEST(NAME, CALL, RES)      \
	auto NAME = [&](const char *n) { \
		if (CALL == RES) {             \
			good(n);                     \
		} else {                       \
			bad(n);                      \
		}                              \
	};                               \
	NAME(#NAME);

void printSummary() {
	for (auto test : testcases) {
		std::cout << "Test: " << test.first << "\t:" << (test.second ? " passed" : " failed") << std::endl;
	}
}

int numTestErrors() {
	int testErrors = 0;
	for (auto test : testcases) {
		if (!test.second) {
			testErrors++;
		}
	}
	return testErrors;
}
