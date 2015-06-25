#ifndef INSTRO_ROSE_ROSEPASS_H
#define INSTRO_ROSE_ROSEPASS_H

#include "instro/core/Pass.h"

namespace InstRO
{

namespace Test	{
	class TestPass : public PassImplementation		{
		public:
			// CI: delete implicit c-tor of the test-pass. Otherwise, it would try to invoce the deted PassImplementation c-tor and cause an error
			TestPass() = delete;
			TestPass(ChannelConfiguration ccfg) :InstRO::PassImplementation(ccfg){};

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