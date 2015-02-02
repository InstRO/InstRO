#include "instro.h"

using namespace InstRO;

int main(int argc,char ** argv)
{
	::InstRO::InstRO * instro;
	instro=new ::InstRO::RoseInstrumentor(&argc,&argv);
	::InstRO::Factory * fac=instro->getFactory();
	::InstRO::Selector * exampleSelector=fac->createExampleSelector();
	instro->printDebugStatus();
	return 0;
}