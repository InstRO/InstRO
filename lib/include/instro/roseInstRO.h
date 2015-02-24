#ifndef ROSEINSTRO_H
#define ROSEINSTRO_H

#include "instro/interface.h"

namespace InstRO
{
	namespace Rose{
		class RosePass: public InstRO::Pass
		{};
		class Selector:public RosePass
		{
		};
		class Adapter:public RosePass
		{
		};
		class Transformer: public RosePass
		{
		};
			class RosePassFactory:public PassFactory
			{
			public:	
				RosePassFactory(PassManagement::PassManager * refManager):PassFactory(refManager){};
				RosePass * createBlackNWhiteFilter(Pass * input)
				{
					return NULL;
				//	RosePass * pass=new RosePass(input);
			//		refToGobalPassManager->registerPass(pass);
					
//					refToGobalPassManager->addDependency(pass,input,1);
					//,std::vector<Pass*>().push_back(input));
				}
				RosePass * createBlackNWhiteSelector(std::string string){return NULL;};
				RosePass * createBooleanOrSelector(Pass * inputA,Pass * inputB){return NULL;};
				RosePass * createProgramEntrySelector(){return NULL;};
				RosePass * createCygProfileAdapter(Pass * input){return NULL;};
			};
		}; // Rose Name Space

	class RoseInstrumentor: public Instrumentor
	{
	public: 
		Rose::RosePassFactory * getFactory(Instrumentor::CompilationPhase phase){return new Rose::RosePassFactory(passManager);}
//		PassFactory * getFactory(In
		void init(){};
		void apply(){};
		void finalize(){};
	};
}
#endif