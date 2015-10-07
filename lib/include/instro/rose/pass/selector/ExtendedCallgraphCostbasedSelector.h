#ifndef INSTRO_ROSE_EXTENDEDCALLGRAPHCOSTBASEDSELECTOR
#define INSTRO_ROSE_EXTENDEDCALLGRAPHCOSTBASEDSELECTOR

namespace InstRO {
namespace Rose {
namespace Selector {

/**
 * This selector selects a subset of function and loop statements, using an extended cfg
 * and a call amount estimation heuristic
 *
 * \ingroup Selector
 * \author Matthias Zoellner
 */
class ExtendedCallgraphCostbasedSelector : public InstRO::Rose::RosePassImplementation {
/* Notes to myself (to be removed when development is done)
input pass retrieval:
ChannelConfiguration::getPasses() - returns vector<Pass*>
 */
public:
	ExtendedCallgraphCostbasedSelector(InstRO::PassManagement::Pass* input);

	void init() override;
	void execute() override;
	void finalize() override;
	void releaseOutput() override;
	InstRO::Core::ConstructSet* getOutput() override;

private:
	InstRO::Core::ConstructSet* output;
}; // class ExtendedCallgraphCostbasedSelector


} // namespace Selector
} // namespace Rose
} // namespace InstRO

#endif // INSTRO_ROSE_EXTENDEDCALLGRAPHCOSTBASEDSELECTOR

