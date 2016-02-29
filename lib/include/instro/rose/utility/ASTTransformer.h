#ifndef INSTRO_ROSE_UTILITY_ASTTRANSFORMER_H
#define INSTRO_ROSE_UTILITY_ASTTRANSFORMER_H

#include "rose.h"

namespace InstRO {
namespace Rose {
namespace Utility {

/**
 * Provides higher level transformation methods to facility mutation of the AST
 */
namespace ASTTransformation {

/**
 * Whenever the transformation should include an include file into source files. The class keeps track of already
 * transformed files.
 */
class HeaderIncludeHelper {
 public:
	static HeaderIncludeHelper &getInstance() {
		static HeaderIncludeHelper instance;
		return instance;
	}

	bool insertHeaderIfSource(std::string headerName, SgLocatedNode *node);

 private:
	HeaderIncludeHelper() {}
	HeaderIncludeHelper(const HeaderIncludeHelper &o) = delete;
	std::map<std::string, std::set<SgSourceFile *> > alreadyIncludedMap;
};

}	// ASTTransformer
}	// Utility
}	// Rose
}	// InstRO

#endif
