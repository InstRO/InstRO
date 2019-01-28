#ifndef INSTRO_CLANG_SUPPORT_CLANGHELPERS_H
#define INSTRO_CLANG_SUPPORT_CLANGHELPERS_H
#include "clang/Tooling/Refactoring.h"

namespace InstRO {

namespace Clang {
namespace Support {
clang::tooling::Replacements mergeToolReplacements(clang::tooling::RefactoringTool &);
}
}	// namespace Clang
}	// namespace InstRO
#endif
