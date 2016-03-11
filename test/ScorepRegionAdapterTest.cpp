
#include "instro.h"


int main(int argc, char **argv){

#if INSTRO_USE_ROSE
	using InstrumentorType = InstRO::RoseInstrumentor;
	InstrumentorType instrumentor(argc, argv);
#endif
#if INSTRO_USE_CLANG
	throw std::string("The Default Instrumentation Adapter is not yet implemeted in Clang");
#endif


	auto ret = InstRO::Utility::getScorepIncludeFlags();
	std::cerr << ret << std::endl;

	auto factory = instrumentor.getFactory();

	auto fDefSelector = factory->createConstructTraitSelector(InstRO::Core::ConstructTraitType::CTFunction);
	auto stmtSelector = factory->createConstructTraitSelector(InstRO::Core::ConstructTraitType::CTStatement);

	auto combinedSelector = factory->createBooleanOrSelector(fDefSelector, stmtSelector);

	auto scorepAdapter = factory->createScorepRegionInstrumentationAdapter(combinedSelector);

	instrumentor.apply();

	return 0;
}
