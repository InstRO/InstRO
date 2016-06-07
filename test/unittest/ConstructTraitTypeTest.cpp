#include "instro/core/ConstructTraitType.h"

#include "TestBasic.h"

int main(int argc, char** argv) {
	using namespace InstRO::Core;
	try {
		TEST(expr, getConstructTraitTypeFromString("Expression"), ConstructTraitType::CTExpression)
		TEST(stmt, getConstructTraitTypeFromString("Statement"), ConstructTraitType::CTStatement)
		TEST(omp, getConstructTraitTypeFromString("OpenMPStatement"), ConstructTraitType::CTOpenMPStatement)
		TEST(loop, getConstructTraitTypeFromString("LoopStatement"), ConstructTraitType::CTLoopStatement)
		TEST(cond, getConstructTraitTypeFromString("ConditionalStatement"), ConstructTraitType::CTConditionalStatement)
		TEST(scope, getConstructTraitTypeFromString("ScopeStatement"), ConstructTraitType::CTScopeStatement)
		TEST(simple, getConstructTraitTypeFromString("SimpleStatement"), ConstructTraitType::CTSimpleStatement)
		TEST(wrappable, getConstructTraitTypeFromString("WrappableStatement"), ConstructTraitType::CTWrappableStatement)
		TEST(func, getConstructTraitTypeFromString("Function"), ConstructTraitType::CTFunction)
		TEST(file, getConstructTraitTypeFromString("FileScope"), ConstructTraitType::CTFileScope)
		TEST(glob, getConstructTraitTypeFromString("GlobalScope"), ConstructTraitType::CTGlobalScope)

		TEST(expr2expr, getConstructTraitTypeFromString(toString(ConstructTraitType::CTExpression)), ConstructTraitType::CTExpression)
		TEST(stmt2stmt, getConstructTraitTypeFromString(toString(ConstructTraitType::CTStatement)), ConstructTraitType::CTStatement)
		TEST(omp2omp, getConstructTraitTypeFromString(toString(ConstructTraitType::CTOpenMPStatement)), ConstructTraitType::CTOpenMPStatement)
		TEST(loop2loop, getConstructTraitTypeFromString(toString(ConstructTraitType::CTLoopStatement)), ConstructTraitType::CTLoopStatement)
		TEST(cond2cond, getConstructTraitTypeFromString(toString(ConstructTraitType::CTConditionalStatement)), ConstructTraitType::CTConditionalStatement)
		TEST(scope2scope, getConstructTraitTypeFromString(toString(ConstructTraitType::CTScopeStatement)), ConstructTraitType::CTScopeStatement)
		TEST(simple2simple, getConstructTraitTypeFromString(toString(ConstructTraitType::CTSimpleStatement)), ConstructTraitType::CTSimpleStatement)
		TEST(wrap2wrap, getConstructTraitTypeFromString(toString(ConstructTraitType::CTWrappableStatement)), ConstructTraitType::CTWrappableStatement)
		TEST(func2func, getConstructTraitTypeFromString(toString(ConstructTraitType::CTFunction)), ConstructTraitType::CTFunction)
		TEST(file2file, getConstructTraitTypeFromString(toString(ConstructTraitType::CTFileScope)), ConstructTraitType::CTFileScope)
		TEST(glob2glob, getConstructTraitTypeFromString(toString(ConstructTraitType::CTGlobalScope)), ConstructTraitType::CTGlobalScope)
	} catch (std::string& str) {
		std::cerr << str << std::endl;
		return 1;
	}
	printSummary();
	return numTestErrors();
}
