#ifndef INSTRO_ROSE_UTILITY_SCOPE_STACK_GUARD_HPP
#define INSTRO_ROSE_UTILITY_SCOPE_STACK_GUARD_HPP

namespace InstRO {

namespace Rose {

namespace Utility {

/// \brief Guard the ScopeStack of SageBuilder against missing function calls.
///
/// The constructor of this class calls SageBuilder::pushScopeStack, the destructor SageBuilder::popScopeStack.
/// \note \c rose.h must be included prior to this header.
/// \author Simon Reuß
class ScopeStackGuard
{
public:
    ScopeStackGuard(SgScopeStatement* scope)
    {
        SageBuilder::pushScopeStack(scope);
    }

    ~ScopeStackGuard()
    {
        SageBuilder::popScopeStack();
    }
};

}
}
}

#endif // INSTRO_ROSE_UTILITY_SCOPE_STACK_GUARD_HPP
