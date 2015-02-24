#define USING_ROSE
#include "instro.h"

//using namespace InstRO;

int main(int argc,char ** argv)
{
	InstRO::Instrumentor * instro=new InstRO::RoseInstrumentor();
	InstRO::PassFactory * factory = instro->getFactory();
	InstRO::Pass * bnwSelector,* programEntrySelector,*orSelector,*adapter;
	bnwSelector=factory->createBlackNWhiteSelector("HelloWorld*");
	programEntrySelector=factory->createProgramEntrySelector();
	orSelector=factory->createBooleanOrSelector(bnwSelector,programEntrySelector);
	adapter=factory->createCygProfileAdapter(orSelector);
	instro->init();
//	::InstRO::Rose::Selector * myDevelopStuff=new ::InstRO::Rose::Selector();
	//instro->getPassManager()->registerPass
	instro->apply();
	instro->finalize();


/*	::InstRO::InstRO * instro;
	//CI: Rose specific constructor for the RoseInstrumentor - factory
	instro=new ::InstRO::RoseInstrumentor(&argc,&argv);
	::InstRO::Factory * fac=instro->getFactory();
	::InstRO::Selector * exampleSelector=fac->createExampleSelector();
	instro->printDebugStatus();

	Pass *PickerA,*PickerB,*Filter;
	*/

	return 0;
}