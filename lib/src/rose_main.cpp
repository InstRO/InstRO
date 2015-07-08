#define USING_ROSE
#include "instro.h"

// #include "instro/misc/VisualizingPassManager.h"

#include <vector>
#include <string>
#include <iostream>

// using namespace InstRO;

int main(int argc, char** argv) {
	try {
		InstRO::Instrumentor* instro = new InstRO::RoseInstrumentor(argc, argv);
		// CI - Reseting Classic Implementation  InstRO::Ext::VisualizingPassManager * passManager=new
		// InstRO::Ext::VisualizingPassManager();
		// CI - Reseting Classic Implementation  instro->setPassManager(passManager);
		auto aFactory = dynamic_cast<InstRO::Rose::RosePassFactory*>(instro->getFactory());

		std::vector<std::string> filterRules1;
		filterRules1.push_back("main");
		filterRules1.push_back("#Foo#");
		filterRules1.push_back("#Foo");
		filterRules1.push_back("myFoo");
		auto nbs1 = aFactory->createNameBasedSelector(filterRules1);
		auto adapter1 = aFactory->createConstructPrinter(nbs1);

		std::vector<std::string> filterRules2;
		filterRules2.push_back("myFoo");
		auto nbs2 = aFactory->createNameBasedSelector(filterRules2);
		auto celev2 = aFactory->createConstructRaisingElevator(nbs2, InstRO::Core::ConstructLevelType::CLFunction);
		auto adapter2 = aFactory->createConstructPrinter(celev2);

		std::vector<std::string> filterRules3;
		filterRules3.push_back("main");
		auto nbs3 = aFactory->createNameBasedSelector(filterRules3);
		aFactory->createConstructPrinter(nbs3);
		//		auto celev3 = aFactory->createConstructRaisingElevator(nbs3,InstRO::Core::ConstructLevelType::CLExpression);
		auto expressions = aFactory->createConstructLoweringElevator(nbs3, InstRO::Core::ConstructLevelType::CLExpression);
		aFactory->createConstructPrinter(expressions);
		auto simpleStatements =
				aFactory->createConstructLoweringElevator(nbs3, InstRO::Core::ConstructLevelType::CLSimpleStatement);
		aFactory->createConstructPrinter(simpleStatements);
		auto statements = aFactory->createConstructLoweringElevator(nbs3, InstRO::Core::ConstructLevelType::CLStatement);
		aFactory->createConstructPrinter(statements);

		//		auto adapter3 = aFactory->createConstructPrinter(celev3);

		instro->init();
		instro->apply();
		instro->finalize();

		// CI - Reseting Classic Implementation   passManager->outputConfiguration("InstRO-CFG.dot");

	} catch (std::string stringBasedException) {
		std::cout << stringBasedException << std::endl;
		std::cout.flush();
	}

	return 0;
}
