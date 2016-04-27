#
# Check for rapidjson library
# and set variables accordingly

AC_DEFUN([AM_CHECK_RAPIDJSON],
[
AC_ARG_WITH([rapidjson],
	[AS_HELP_STRING([--with-rapidjson@<:@=ARG@:>@],
	[support for parsing json configuration files @<:@default=check@:>@])],
	[],
	[with_rapidjson=check])

RAPIDJSON_INCLUDEDIR=
found_rapidjson=0
AS_IF([test "x$with_rapidjson" != xno],
[AS_IF([test "x$with_rapidjson" = xyes -o "x$with_rapidjson" = xcheck],
[AC_CHECK_HEADER([rapidjson/document.h],[found_rapidjson=1],[found_rapidjson=0])],
[AC_CHECK_FILE([$with_rapidjson/include/rapidjson/document.h],[found_rapidjson=1 ; RAPIDJSON_INCLUDEDIR=$with_rapidjson/include],[found_rapidjson=0])])
AS_IF([test "x$with_rapidjson" != xcheck -a $found_rapidjson = 0],
[AC_MSG_FAILURE([--with-rapidjson was given, but test for the header file failed])])
])

AM_CONDITIONAL(OPT_RAPIDJSON, test x"$with_rapidjson" != xno -a $found_rapidjson = 1)
AC_SUBST(RAPIDJSON_INCLUDEDIR)
])

