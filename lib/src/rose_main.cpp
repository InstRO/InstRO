#define USING_ROSE
#include "instro.h"

// #include "instro/misc/VisualizingPassManager.h"

#include <vector>
#include <string>
#include <iostream>

// using namespace InstRO;

int main(int argc, char** argv) {
	try {
		auto instro = new InstRO::RoseInstrumentor(argc, argv);
		// CI - Reseting Classic Implementation  InstRO::Ext::VisualizingPassManager * passManager=new
		// InstRO::Ext::VisualizingPassManager();
		// CI - Reseting Classic Implementation  instro->setPassManager(passManager);
		auto aFactory = dynamic_cast<InstRO::Rose::RosePassFactory*>(instro->getFactory());

		//		std::vector<std::string> filterRules1;
		//		filterRules1.push_back("main");
		//		filterRules1.push_back("#Foo#");
		//		filterRules1.push_back("#Foo");
		//		filterRules1.push_back("myFoo");
		//		auto nbs1 = aFactory->createIdentifyerSelector(filterRules1);
		//		auto adapter1 = aFactory->createConstructPrinter(nbs1);

		//		std::vector<std::string> filterRules2;
		//		filterRules2.push_back("myFoo");
		//		auto nbs2 = aFactory->createIdentifyerSelector(filterRules2);
		//		auto celev2 = aFactory->createConstructRaisingElevator(nbs2, InstRO::Core::ConstructTraitType::CTFunction);
		//		auto adapter2 = aFactory->createConstructPrinter(celev2);

		//		std::vector<std::string> filterRules3;
		//		filterRules3.push_back("main");
		//		auto nbs3 = aFactory->createIdentifyerSelector(filterRules3);
		//		aFactory->createConstructPrinter(nbs3);
		//		auto celev3 = aFactory->createConstructRaisingElevator(nbs3,InstRO::Core::ConstructLevelType::CLExpression);
		//		auto expressions = aFactory->createConstructLoweringElevator(nbs3,
		// InstRO::Core::ConstructTraitType::CTExpression);
		//		aFactory->createConstructPrinter(expressions);
		//		auto simpleStatements =
		//				aFactory->createConstructLoweringElevator(nbs3, InstRO::Core::ConstructTraitType::CTSimpleStatement);
		//		aFactory->createConstructPrinter(simpleStatements);
		//		auto statements = aFactory->createConstructLoweringElevator(nbs3,
		// InstRO::Core::ConstructTraitType::CTStatement);
		//		aFactory->createConstructPrinter(statements);

		auto getMain = aFactory->createProgramEntrySelector();
		auto expressions =
				aFactory->createConstructLoweringElevator(getMain, InstRO::Core::ConstructTraitType::CTExpression);
		auto astPrinter = aFactory->createConstructHierarchyASTDotGenerator(expressions, "CSHierarchyAST.dot");

		//		auto adapter3 = aFactory->createConstructPrinter(celev3);

		//		auto programEntry = aFactory->createProgramEntrySelector();
		//		aFactory->createConstructPrinter(programEntry);

		instro->init();
		instro->apply();
		instro->finalize();

		// CI - Reseting Classic Implementation   passManager->outputConfiguration("InstRO-CFG.dot");

	} catch (std::string& stringBasedException) {
		std::cerr << "Caught Exception:" << std::endl;
		std::cerr << stringBasedException << std::endl;
		std::cerr.flush();
	}

	return 0;
}
