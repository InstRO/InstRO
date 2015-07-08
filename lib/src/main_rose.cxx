#define USING_TEST
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
		InstRO::Instrumentor * instro = new InstRO::TestInstrumentor();
		// CI - Reseting Classic Implementation  InstRO::Ext::VisualizingPassManager * passManager=new InstRO::Ext::VisualizingPassManager();
		// CI - Reseting Classic Implementation  instro->setPassManager(passManager);
		
		InstRO::Test::RosePassFactory * aFactory=dynamic_cast<InstRO::Test::TestPassFactory*>(instro->getFactory());
		std::vector<std::string> filterRules;
		filterRules.push_back("main");
		InstRO::Pass * aPass= aFactory->createBlackAndWhiteListSelector(filterRules);
		InstRO::Pass * bPass= aFactory->createBlackAndWhiteListSelector(filterRules);
		InstRO::Pass * cPass= aFactory->createBlackAndWhiteListSelector(filterRules);
		InstRO::Pass * compound=aFactory->createBooleanOrSelector(aPass,bPass);

		// Get the configuration container
		InstRO::Rose::RosePassFactory::GenericAdapterConfiguration gac;
		// Add the pass responsible for selecting function for instrumentation
		gac.instrumentFunctions(compound);
		// Add the pass for loop-construct selection to the loop input-channel
		gac.instrumentLoopConstruct(cPass);
		
		InstRO::Pass * adapter= aFactory->createGenericAdapter(gac);

		// CI - Reseting Classic Implementation   passManager->outputConfiguration("InstRO-CFG.dot");
		
	}
	catch(std::string stringBasedException)
	{
		std::cout << stringBasedException<<std::endl;
		std::cout.flush();
	}



	return 0;
}
