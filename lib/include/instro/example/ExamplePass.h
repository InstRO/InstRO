#ifndef INSTRO_ROSE_ROSEPASS_H
#define INSTRO_ROSE_ROSEPASS_H

#include "instro/core/Pass.h"

namespace InstRO
{

	namespace Example	{
		class ExamplePass : public InstRO::Core::PassImplementation		{
		public:
			// CI: delete implicit c-tor of the test-pass. Otherwise, it would try to invoce the deted PassImplementation c-tor and cause an error
			ExamplePass() = delete;
			ExamplePass(InstRO::Core::ChannelConfiguration ccfg) :InstRO::Core::PassImplementation(ccfg){};


		};
namespace Adapters	{
}

namespace Selectors	{
		}
namespace Transformer	{
		}
	}
};
#endif