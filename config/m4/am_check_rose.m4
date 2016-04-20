#
# Checks for Rose and sets variable accordingly
#

AC_DEFUN([AM_CHECK_ROSE],
[
AC_ARG_WITH(rose,
[AS_HELP_STRING([--with-rose@<:@=ARG@:>@],[enable use of ROSE and optionally specify a non-standard install prefix @<:@default=no@:>@])]
,[],[with_rose=no])

ROSE_INCLUDEDIR=
ROSE_LIBDIR=
found_rose=0
AS_IF([test x"$with_rose" != xno],
[AS_IF([test x"$with_rose" = xyes],
[AC_CHECK_HEADER([rose.h],[found_rose=1],[found_rose=0])],
[ROSE_LIBDIR=$with_rose/lib
AC_CHECK_FILE([$with_rose/include/rose.h],[found_rose=1 ; ROSE_INCLUDEDIR=$with_rose/include],[found_rose=0
AC_CHECK_FILE([$with_rose/include/rose/rose.h],[found_rose=1 ; ROSE_INCLUDEDIR=$with_rose/include/rose],[found_rose=0])])])
AS_IF([test $found_rose = 0],[AC_MSG_FAILURE([Failed to find the ROSE library])])],)

## set @ROSE_INCLUDEDIR@ and @ROSE_LIBDIR@ variables
AC_SUBST(ROSE_INCLUDEDIR)
AC_SUBST(ROSE_LIBDIR)
])
