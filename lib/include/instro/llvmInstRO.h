#ifndef ROSEINSTRO_H
#define ROSEINSTRO_H

#include "instro/interface.h"
namespace InstRO
{
	namespace LLVM {
		class Pass : public ::InstRO::Pass {

		};

		class PassFactory : public ::InstRO::PassFactory {

		};

		class InstRO : public ::InstRO::InstRO {
			public:
				PassFactory * getFactory();
		};
	}
}
#endif
