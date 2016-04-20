#include "instro/rose/utility/ASTTransformer.h"
#define INSTRO_LOG_LEVEL DEBUG
#include "instro/utility/Logger.h"

namespace InstRO {
namespace Rose {
namespace Utility {
namespace ASTTransformation {

bool HeaderIncludeHelper::insertHeaderIfSource(std::string headerName, SgLocatedNode* node) {
	SgSourceFile* sourceFile = isSgSourceFile(SageInterface::getEnclosingFileNode(node));
	std::string fileInfoName = node->get_file_info()->get_filenameString();
	std::string sourceFileName = sourceFile->getFileName();

	if(alreadyIncludedMap.find(headerName) == alreadyIncludedMap.end()){
		alreadyIncludedMap.insert({headerName, std::set<SgSourceFile *>()});
		logIt(DEBUG) << "Inserted new element for " << headerName << " into map" << std::endl;
	}

	auto &filesWithInclude = alreadyIncludedMap.at(headerName);

	// if the two names do not match, the construct originates from an include
	if (sourceFile != nullptr && (fileInfoName == sourceFileName)) {
		if (filesWithInclude.find(sourceFile) == filesWithInclude.end()) {
			SageInterface::insertHeader(headerName, PreprocessingInfo::before, true,
																	sourceFile->get_globalScope());
			filesWithInclude.insert(sourceFile);
			logIt(INFO) << "RoseCodeWrapper: inserted header in " << sourceFileName << std::endl;
		}
		return true;
	}
	logIt(WARN) << "RoseCodeWrapper: header instrumentation is not supported. " << fileInfoName << std::endl;
	return false;
}
}
}
}
}
