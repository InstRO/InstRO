
#include "instro.h"
#include <iostream>


#ifdef USE_ROSE
#include "lib/RoseTestSupport.h"
#endif

/**
 * This is the TestInstrumentor implementation.
 */
int main(int argc, char **argv) {
/*
 * We want to use the same binary for both Rose and Clang
 */
#ifdef USE_ROSE
	using InstrumentorType = RoseTest::RoseTestInstrumentor;
#endif

	InstrumentorType instrumentor(argc, argv);
	auto factory = instrumentor.getFactory();

	std::set<std::string> expItems;
	expItems.insert("emptyMain.cpp:3--Function-main");

	auto sel = factory->createConstructClassSelector(InstRO::Core::ConstructTraitType::CTFunction);
	auto testAdapter = factory->createTestAdapter(sel, expItems);

	instrumentor.apply();

	return 0;
}
