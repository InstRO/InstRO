#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/CodeInsertionHelper.h"


using namespace InstRO;


CodeInsertionHelper::CodeInsertionHelper() {
	assert(false);
}


CodeInsertionHelper::CodeInsertionHelper(SgDeclarationStatement* anchor)
	: currentAnchors(1, std::make_pair(anchor, after)) {

}


CodeInsertionHelper::CodeInsertionHelper(SgStatement* anchor, RelativePosition insertionPosition)
	: currentAnchors(1, std::make_pair(anchor, insertionPosition)) {

}


CodeInsertionHelper::CodeInsertionHelper(const std::vector<std::pair<SgStatement*, RelativePosition> >& anchorList)
	: currentAnchors(anchorList) {

}


CodeInsertionHelper::CodeInsertionHelper(const std::vector<SgDeclarationStatement*>& anchorList) {
	for (auto i : anchorList) {
		currentAnchors.push_back(std::make_pair(i, after));
	}
}



CodeInsertionHelper::CodeInsertionHelper(const CodeInsertionHelper& other)
	: currentAnchors(other.currentAnchors) {

}


CodeInsertionHelper& CodeInsertionHelper::operator =(const CodeInsertionHelper& other) {

	currentAnchors = other.currentAnchors;

	return *this;
}


CodeInsertionHelper::~CodeInsertionHelper() {

}


void CodeInsertionHelper::pushStatement(SgStatement* node) {

	// Copy is only necessary for multiple insertion points
	// and there are some problems comming with the copies
	bool needCopy = false;

	std::vector<std::pair<SgStatement*, RelativePosition> >::iterator it;
	for (it = currentAnchors.begin(); it != currentAnchors.end(); ++it) {

		SgStatement* copy = node;

		if (needCopy) {
			copy = SageInterface::deepCopy(node);
		}

		switch (it->second) {
		case RelativePosition::before:
			SageInterface::insertStatementBefore(it->first, copy);
			break;
		case RelativePosition::after:
			SageInterface::insertStatementAfter(it->first, copy);
			break;
		case RelativePosition::prepend:
			assert(isSgScopeStatement(it->first));
			SageInterface::prependStatement(copy, isSgScopeStatement(it->first));
			break;
		case RelativePosition::append:
			assert(isSgScopeStatement(it->first));
			SageInterface::appendStatement(copy, isSgScopeStatement(it->first));
			break;
		}

		it->first = copy;
		it->second = RelativePosition::after;

		needCopy = true;
	}
}

