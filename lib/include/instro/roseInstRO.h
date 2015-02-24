#ifndef ROSEINSTRO_H
#define ROSEINSTRO_H

#include "instro/interface.h"
namespace InstRO
{
	namespace ROSE
	{
		class RoseConstructSet:public ::InstRO::ConstructSet{};
		
		class RosePass: public Pass{
			public:
				RoseConstructSet* registerInput(int maxConstructLevel,Pass *);
				RoseConstructSet* registerOutput(int minConstructLevel){
					return dynamic_cast<::InstRO::ROSE::RoseConstructSet*>(::InstRO::Pass::registerOutput(minConstructLevel));
				//	::InstRO::Pass::registerOutput(minConstructLevel);
				};
		};
		class RoseSelector:public Selectors::Selector,public RosePass {};


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