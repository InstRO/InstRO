#include "instro/test/TestPass.h"


namespace InstRO
{
	namespace Test
	{
		namespace Selectors
		{
			class CallPathSelector :public TestPass{
			protected:
				Pass * fromPass, *toPass;
				CallPathSelector() = delete;
			public:
				CallPathSelector(Pass * from, Pass * to):fromPass(from),toPass(to){};
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