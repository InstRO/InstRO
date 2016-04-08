#                                               -*- Autoconf -*-
# Followed instructions from
# http://www.sourceware.org/autobook/autobook/autobook_32.html#SEC32

# -------------------------------
# 1) Boilerplate
# -------------------------------


# -------------------------------
# 1.1 Init the Autoconf
# -------------------------------
AC_PREREQ([2.63])
AC_INIT([InstRO], [1.0], [christian.iwainsky@sc.tu-darmstadt.de],[instro])

# 1.01 Set the aclocal

# -------------------------------
# 1.2 Set the directories
# -------------------------------
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config/m4])
AM_INIT_AUTOMAKE([foreign subdir-objects])
PKG_PROG_PKG_CONFIG

# defined in ./config/m4/am_dir_expand
AM_DIR_EXPAND

# -------------------------------
# 1.3 config.h
# -------------------------------
# As Automake prefers its version, let automake generate the header
AC_CONFIG_SRCDIR([lib/include/instro.h])
AM_CONFIG_HEADER([config.h])

# -------------------------------
# Checks for programs.
# -------------------------------
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# -------------------------------
# Check for functions
# -------------------------------
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strerror])

# -------------------------------
# 1.35 automake
# -------------------------------
AC_PROG_RANLIB

# -------------------------------
# 1.4 get Libtool started
# -------------------------------
# aclocal needs this to add libtool:
AC_PROG_LIBTOOL([])
LT_INIT

# -------------------------------
# 1.5 Autoheader Options
# -------------------------------
AH_TOP([
#ifndef _CONFIG_H
#define _CONFIG_H
])
AH_BOTTOM([
#endif /* _CONFIG_H */
])


# -------------------------------
# 2) Configure options
# -------------------------------

# -------------------------------
# 2.0.0) Add debug option
# -------------------------------
AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
               [enable debugging, default: no]),
[case "${enableval}" in
             yes) ;;
             no) ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[enable_debug=no])

AM_CONDITIONAL(DEBUG, test x"$enable_debug" = x"yes")


# -------------------------------
# 2.0.1) Add examples option
# -------------------------------

AC_ARG_ENABLE(examples, 
	AS_HELP_STRING([--enable-examples], [build examples, default: no]),
	[case "${enableval}" in 
						yes) ;;
						no) ;;
			esac],[enable_examples=no])

AM_CONDITIONAL(BUILD_EXAMPLES, [test x"$enable_examples" = x"yes"])

# -------------------------------
# 2.1) add ROSE
# defined in ./config/m4/am_check_rose.m4
# -------------------------------
AM_CHECK_ROSE
AC_MSG_NOTICE(["Rose libs at $ROSE_LIBDIR"])
AC_MSG_NOTICE(["Rose includes at $ROSE_INCLUDEDIR"])

# ---------------------------
# 2.2) Add LLVM/Clang toolchain
# XXX It is a little unfortunate but it seems that we need access to the source as well as the build/install
# directory of llvm/clang.
# defined in ./config/m4/am_check_clang.m4
# ---------------------------
AM_CHECK_CLANG
AC_MSG_NOTICE(["Clang Libs: $CLANG_LIBS_LIST"])
AC_MSG_NOTICE(["LLVM install path has been set to $LLVM_INSTALL_PATH"])

# ------
# Score P installation
# ------
AC_ARG_WITH([scorep], 
						[AS_HELP_STRING([--with-scorep@<:@=ARG@:>@], 
						[Path to the Score-P installation @<:@default=no@:>@])],
						[], [with_scorep=no])

AM_CONDITIONAL(WITHSCOREP, test x"$with_scorep" != xno)
AC_SUBST(WITHSCOREP)

# -------------------------------
# 4) Checks for libraries.
# -------------------------------

# -------------------------------
# 4.1) add BOOST
# -------------------------------
AX_BOOST_BASE([1.47])
AC_MSG_NOTICE(["ac_boost_path=$ac_boost_path"])
AC_MSG_NOTICE(["BOOST_CPPFLAGS=$BOOST_CPPFLAGS"])
AC_MSG_NOTICE(["BOOST_LDFLAGS=$BOOST_LDFLAGS"])

AX_BOOST_IOSTREAMS
AX_BOOST_SYSTEM
AX_BOOST_REGEX
# AX_BOOST_REQUIRE([1.47])
BOOST_INCLUDEDIR="$ac_boost_path/include"
BOOST_LIBDIR="$ac_boost_path/lib"

# Export Boost variables
AC_SUBST(BOOST_INCLUDEDIR)
AC_SUBST(BOOST_LIBDIR)

# -------------------------------
# check for rapidjson
# defined in ./config/m4/am_check_rapidjson.m4
# -------------------------------
AM_CHECK_RAPIDJSON

AM_CONDITIONAL(USEROSE, test x"$with_rose" != xno)
AM_CONDITIONAL(USECLANG, test x"$with_llvm_src" != xno)

AC_SUBST(CONFIG_DISABLEEXCEPTIONS, [$found_llvm])
AC_SUBST(CONFIG_USE_ROSE, [$found_rose])
AC_SUBST(CONFIG_USE_CLANG, [$found_llvm])
AC_SUBST(CONFIG_INSTALL_DIR, [$prefix])

# -------------------------------
# 4.9) Set Variables.
# -------------------------------

ROSE_CPPFLAGS="$BOOST_CPPFLAGS -I$ROSE_INCLUDEDIR -I$ROSE_INCLUDEDIR/rose"

BOOST_LINK_LIBRARIES="-L$BOOST_LIBDIR $BOOST_IOSTREAMS_LIB $BOOST_SYSTEM_LIB $BOOST_REGEX_LIB"
ROSE_LINK_LIBRARIES="-L$ROSE_LIBDIR -lrose"

AC_MSG_NOTICE(["Boost: BOOST_IOSTREAMS == $BOOST_IOSTREAMS_LIB"])
AC_MSG_NOTICE(["Boost: BOOST_SYSTEM == $BOOST_SYSTEM_LIB"])
AC_MSG_NOTICE(["Boost: BOOST_REGEX == $BOOST_REGEX_LIB"])
AC_MSG_NOTICE(["ROSE: ROSE_LINK_LIBRARIES := $ROSE_LINK_LIBRARIES"])
AC_MSG_NOTICE(["aux path is set to: $am_aux_dir"])
AC_MSG_NOTICE(["am build path is set to: $am_build_dir"])

AC_SUBST(ROSE_CPPFLAGS)
AC_SUBST(ROSE_CFLAGS)
AC_SUBST(ROSE_CXXFLAGS)

AC_SUBST(BOOST_LINK_LIBRARIES)
AC_SUBST(ROSE_LINK_LIBRARIES)

AC_SUBST(am_aux_dir)
AC_SUBST(am_build_dir)
# -------------------------------
# 5) Checks for header files.
# -------------------------------
AC_CHECK_HEADERS([stdlib.h])

# -------------------------------
# 6) Checks for typedefs, structures, and compiler characteristics.
# -------------------------------
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# -------------------------------
# 7) Checks for library functions.
# -------------------------------
AC_CHECK_FUNCS([select])

# Makefiles wich are always generated
AC_CONFIG_FILES([Makefile
		lib/Makefile
		lib/include/instro/config.h
		support/Makefile
		test/Makefile
	])

# Build the shipped examples conditionally
AM_COND_IF([BUILD_EXAMPLES],[
	AC_CONFIG_FILES([
			examples/Makefile
			examples/ConfigurationLoader/Makefile
			examples/ClangSrcToSrcFunctionInstrumentor/Makefile
		])
])

# -------------------------------
# 8) generate Makefiles from Makefil.in
# -------------------------------
AC_OUTPUT
