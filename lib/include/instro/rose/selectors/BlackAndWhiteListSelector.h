#ifndef ROSE_BLACKANDWHITELISTSELECTOR_H
#define ROSE_BLACKANDWHITELISTSELECTOR_H
#include <vector>
#include <string>
#include "instro/rose/RosePass.h"
#include "instro/core/PassImplementation.h" // ChannelConfiguration
namespace InstRO
{
namespace Rose
{
namespace Selectors
{

class BlackAndWhiteListSelector:public RosePass{
public:
	// Empty dependency
	BlackAndWhiteListSelector(std::vector<std::string> matchRules):RosePass(InstRO::Core::ChannelConfiguration()){

	};
	void execute(){};
	void init(){};
	void finalize(){};
	void releaseOutput(){};
	InstRO::Core::ConstructSet * getOutput() override{
		return new InstRO::Core::ConstructSet(); }
};
}// End namespace Selectors
}// End namespace Rose
}// End namepsace InstRO
#endif