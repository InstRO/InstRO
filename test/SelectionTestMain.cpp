#include "instro.h"
#include "lib/TestAdapter.h"
#include <iostream>

namespace RoseTest{

/**
 * Some prototyping
 */
class RoseTestFactory : public InstRO::Rose::RosePassFactory {
	public:
		RoseTestFactory(InstRO::PassManagement::PassManager *manager, SgProject *project) : InstRO::Rose::RosePassFactory(manager, project){}
		InstRO::Pass *createTestAdapter(InstRO::Pass *input, std::set<std::string> expectedItems){
			auto pImpl = new InstRO::Test::TestAdapter(input, expectedItems);
			InstRO::Pass *p = new InstRO::Pass(pImpl);
			p->setPassName("TestAdapter");
			passManager->registerPass(p);
			return p;
		}
};

class RoseTestInstrumentor : public InstRO::RoseInstrumentor {
	public:
		RoseTestInstrumentor(int argc, char **argv) : InstRO::RoseInstrumentor(argc, argv) {}

		RoseTestFactory *getFactory(InstRO::Instrumentor::CompilationPhase phase = InstRO::Instrumentor::CompilationPhase::frontend) override {
			return new RoseTestFactory(passManager, project);
		}

};
}

/**
 * This is the TestInstrumentor implementation.
 */
int main(int argc, char **argv){

/*
 * We want to use the same binary for both Rose and Clang
 */
#ifdef USE_ROSE
	using InstrumentorType = RoseTest::RoseTestInstrumentor;
#endif

	auto instrumentor = new InstrumentorType(argc, argv);
	auto factory = instrumentor->getFactory();

	std::set<std::string> expItems;
	expItems.insert("main.cpp:3");

	auto sel = factory->createConstructClassSelector(InstRO::Core::ConstructTraitType::CTFunction);
	auto testAdapter = factory->createTestAdapter(sel, expItems);

	instrumentor->apply();

	return 0;
}
