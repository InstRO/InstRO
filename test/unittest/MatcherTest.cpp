#include "instro/tooling/NamedConstructAccess.h"

#include "TestBasic.h"

int main(int argc, char **argv) {
	bool passed(true);

	InstRO::Tooling::NamedConstructAccess::WildcardedStringMatcher m;
	TEST(foo01, m.isMatch("foo", "foo"), true)

	TEST(foo02, m.isMatch("foo", "#foo"), true)

	TEST(foo03, m.isMatch("foo", "foo#"), true)

	TEST(foo04, m.isMatch("foo", "f#o"), true)

	TEST(foo05, m.isMatch("foo", "#o"), true)

	TEST(foo06, m.isMatch("foo", "#f#"), true)

	TEST(foo07, m.isMatch("foo", "#foo#"), true)

	TEST(foo08, m.isMatch("foo", "#f#o#o#"), true)

	TEST(foo09, m.isMatch("foo", "Foo"), false)

	TEST(foo10, m.isMatch("foo", "Bar"), false)

	TEST(foo11, m.isMatch("foo", "#:foo"), false)

	TEST(foo12, m.isMatch("foo", "fo"), false)

	TEST(foo13, m.isMatch("foo", "#foo#bar"), false)

	TEST(foo14, m.isMatch("foo", "foo#var"), false)

	TEST(foo15, m.isMatch("foo", "fooo"), false)

	TEST(foo16, m.isMatch("foo", "foo#o#"), false)

	TEST(foo17, m.isMatch("foo", "#######f#####"), true)

	TEST(foo18, m.isMatch("foo", "#"), true)

	TEST(foo19, m.isMatch("foo", ""), false)

	TEST(foo20, m.isMatch("foo", "f#########o"), true)

	TEST(foo21, m.isMatch("f o o ", "#f#"), true);

	printSummary();
	return numTestErrors();
}
