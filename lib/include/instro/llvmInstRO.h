#ifndef ROSEINSTRO_H
#define ROSEINSTRO_H

#include "instro/interface.h"
namespace InstRO
{
	namespace LLVM
	{
		class LLVMSelector:public Selector
		{};
		class ProgramEntrySelector:public ::InstRO::Selectors::ProgramEntrySelector, public LLVMSelector
		{
		};
		class LLVMExampleSelector:public Selector
		{

		};

		class LLVMFactory: public Factory
		{
			LLVMExampleSelector * createExampleSelector()
			{
					return new LLVMExampleSelector();
			}
		};
	}
	class LLVMInstrumentor:public InstRO
	{
	public:
		LLVMInstrumentor(int * argc,char**argv)
		{
		};
		LLVM::LLVMFactory * getFactory(){};
	protected:

	private:

	};
}
#endif