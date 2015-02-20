#ifndef ROSEINSTRO_H
#define ROSEINSTRO_H

#include "instro/interface.h"
namespace InstRO
{
	namespace ROSE
	{
		class RoseSelector:public Selector
		{};

		class ProgramEntrySelector:public ::InstRO::Selectors::ProgramEntrySelector, public RoseSelector
		{
		};

		class RoseExampleSelector:public RoseSelector
		{

		};

		class RoseFactory: public Factory
		{
			RoseExampleSelector * createExampleSelector()
			{
					return new RoseExampleSelector();
			}
		};
	}


	class RoseInstrumentor:public InstRO
	{
	public:
		RoseInstrumentor(int * argc,char***argv)
		{
			factory=new ROSE::RoseFactory();
		};
		ROSE::RoseFactory * getFactory(){return factory;};

		virtual void printDebugStatus()
		{
			std::cout <<"RoseInstrumentor Class" << std::endl;
			::InstRO::InstRO::printDebugStatus();
		}
	protected:

	private:
		ROSE::RoseFactory * factory;

	};
}
#endif