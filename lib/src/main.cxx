#define USING_EXAMPLE
#include "instro.h"

#include "instro/misc/VisualizingPassManager.h"


#include <vector>
#include <string>
#include <iostream>

//using namespace InstRO;



int main(int argc,char ** argv)
{
	try
	{
		InstRO::Instrumentor * instro = new InstRO::ExampleInstrumentor();
		// CI - Reseting Classic Implementation  InstRO::Ext::VisualizingPassManager * passManager=new InstRO::Ext::VisualizingPassManager();
		// CI - Reseting Classic Implementation  instro->setPassManager(passManager);
		
		auto aFactory = dynamic_cast<InstRO::Example::ExamplePassFactory*>(instro->getFactory());

		std::vector<std::string> filterRules;
		filterRules.push_back("main");
		auto aPass= aFactory->createStringBasedSelector(filterRules);
		auto bPass = aFactory->createStringBasedSelector(filterRules);
		auto compound = aFactory->createBooleanOrSelector(aPass, bPass);
		
		auto adapter = aFactory->createGPIAdapter(compound);

		// CI - Reseting Classic Implementation   passManager->outputConfiguration("InstRO-CFG.dot");
		
	}
	catch(std::string stringBasedException)
	{
		std::cout << stringBasedException<<std::endl;
		std::cout.flush();
	}



	/*
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

	*/
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