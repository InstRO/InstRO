#include "instro/core/PassImplementation.h"
#include "instro/test/TestPass.h"


namespace InstRO
{

	namespace Test
	{
		namespace Selectors
		{
			class CallPathSelector :public TestPass{
				void foor(){}
			protected:
				Pass * fromPass, *toPass;
				CallPathSelector() = delete;
			public:
				CallPathSelector(Pass * from, Pass * to):InstRO::Core::PassImplementation(ChannelConfiguration()), fromPass(from), toPass(to){};
				virtual void init() override;
				virtual void execute() override;
				virtual void finalize() override ;
				virtual void releaseOutput() override;
				virtual Core::ConstructSet *getOutput() override ;
			protected:
				Core::ConstructSet output;

			};
		}

	}
}