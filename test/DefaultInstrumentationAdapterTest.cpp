
#include "instro.h"


int main(int argc, char **argv){

#if INSTRO_USE_ROSE
	using InstrumentorType = InstRO::RoseInstrumentor;
	InstrumentorType instrumentor(argc, argv);
#endif
#if INSTRO_USE_CLANG
	throw std::string("The Default Instrumentation Adapter is not yet implemeted in Clang");
#endif

	auto factory = instrumentor.getFactory();

	auto fDefSelector = factory->createConstructClassSelector(InstRO::Core::ConstructTraitType::CTFunction);
	auto defaultAdapter = factory->createDefaultInstrumentationAdapter(fDefSelector);

	instrumentor.apply();

	return 0;
}
