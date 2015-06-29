#include "instro/core/PassImplementation.h"
#include "instro/example/TestPass.h"


namespace InstRO
{

	namespace Example
	{
		namespace Selectors
		{
			class CallPathSelector :public ExamplePass{
			protected:
				Pass * fromPass, *toPass;
				CallPathSelector() = delete;
			public:
				CallPathSelector(Pass * from, Pass * to) :ExamplePass(Core::ChannelConfiguration(from, to)), fromPass(from), toPass(to){};
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