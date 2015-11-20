#include "instro/tooling/NamedConstructAccess.h"
#include <map>
#include <string>
#include <cassert>
#include <iostream>


//#define EXPECT(CALL, RES) assert(RES == CALL);

std::map<std::string, bool> testcases;

void good(std::string testName) { testcases[testName] = true; }

void bad(std::string testName) { testcases[testName] = false; }

#define TEST(NAME, CALL, RES)      \
	auto NAME = [&](const char *n) { \
		if (CALL == RES) {             \
			good(n);                   \
		} else {                       \
			bad(n);                    \
		}                              \
	};                               \
	NAME(#NAME);


int main(int argc, char **argv) {
	bool passed(true);

	auto m = InstRO::Tooling::NamedConstructAccess::WildcardedStringMatcher({"foo"});

	TEST(foo1, m.isMatch("foo"), true)
	TEST(foo2, m.isMatch("#foo"), true)
	TEST(foo3, m.isMatch("foo#"), true)
	TEST(foo4, m.isMatch("f#o"), true)
	TEST(foo5, m.isMatch("#o"), true)
	TEST(foo6, m.isMatch("#f#"), true)
	TEST(foo7, m.isMatch("#foo#"), true)
	TEST(foo8, m.isMatch("#f#o#o#"), true)

	TEST(foo9, m.isMatch("Foo"), false)
	TEST(foo10, m.isMatch("Bar"), false)
	TEST(foo11, m.isMatch("#:foo"), false)
	TEST(foo12, m.isMatch("fo"), false)

	for(auto test : testcases){
		std::cout << "Test: " << test.first << "\t:" << (test.second?" passed":" failed") << std::endl;
		if (!test.second) {
			passed = false;
		}
	}

	return passed;
}
