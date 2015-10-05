
#include "rose.h"
#include "callgraphmanager.h"
#include "../selectors/callfrequencyheuristicselector.h"

namespace InstRO {

class OverheadEstimator : public AstSimpleProcessing {
 public:
	OverheadEstimator(SgProject* project, Selector* selector)
			: selector(selector),
				cgm(project),
				approxAdditionalCycles(0),
				approxAdditionalNanos(0),
				numberOfFunctions(0),
				numberOfFunctionsToInstrument(0),
				additionalCycles(42),
				additionalNanos(12345){};

	void visit(SgNode* node);

	void printResult();
	void writeFile();

 private:
	Selector* selector;
	CallGraphManager cgm;

	std::vector<SgFunctionDeclaration*> fNames;

	long long approxAdditionalCycles;
	long long approxAdditionalNanos;

	long numberOfFunctions;
	long numberOfFunctionsToInstrument;

	/* Estimated costs for different things like instrumentation */
	const int additionalCycles;	// additional cycles needed for a instrumented function
	const long additionalNanos;	// additional ns needed for a instrumented function
};

class OverheadEstimator2 : public CallFrequencyHeuristicSelector {
 public:
	OverheadEstimator2(Selector* selector, SgProject* project)
			: CallFrequencyHeuristicSelector(project, 1, 1), selector(selector){};

	void estimate();
	void printResult();
	void writeFile();

 private:
	Selector* selector;
	long long approxAdditionalCycles;
	long long approxAdditionalNanos;
	long long numberOfFunctionsToInstrument;
	std::vector<SgFunctionDeclaration*> fNames;
};
}
