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
AC_INIT([instro], [0.3], [christian.iwainsky@sc.tu-darmstadt.de],[instro])

# 1.01 Set the aclocal

# -------------------------------
# 1.2 Set the directories
# -------------------------------
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config/m4])
AM_INIT_AUTOMAKE([foreign subdir-objects])
PKG_PROG_PKG_CONFIG

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
# -------------------------------
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

# ---------------------------
# 2.2) Add LLVM/Clang toolchain
# XXX It is a little unfortunate but it seems that we need access to the source as well as the build/install
# directory of llvm/clang.
# ---------------------------
AC_ARG_WITH([llvm-src], 
						[AS_HELP_STRING([--with-llvm-src@<:@=ARG@:>@], 
						[Tells the build process the toplevel directory of the llvm/clang sources @<:@default=no@:>@])], 
						[],[with_llvm_src=no])
if test x"$with_llvm_src" != xno ; then
	LLVM_SRC_PATH="$with_llvm_src"
	CLANG_SRC_INCLUDE_PATH=$LLVM_SRC_PATH/tools/clang/include
	LLVM_SRC_INCLUDE_PATH=$LLVM_SRC_PATH/include

	AC_MSG_NOTICE(["Clang includes are searched in source tree at $CLANG_SRC_INCLUDE_PATH"])
	AC_MSG_NOTICE(["LLVM includes are searched in source tree at $LLVM_SRC_INCLUDE_PATH"])

else
	LLVM_SRC_PATH="UDEF"
	echo "No llvm top level source directory set"
fi

AC_ARG_WITH([llvm-install],
						[AS_HELP_STRING([--with-llvm-install@<:@=ARG@:>@],
						[Tells the build process the toplevel directory of the llvm/clang build @<:@default=no@:>@])],
						[],[with_llvm_install=no])
if test x"$with_llvm_install" != xno ; then
	LLVM_INSTALL_PATH="$with_llvm_install"
	CLANG_INSTALL_INCLUDE_PATH=$LLVM_INSTALL_PATH/include
	LLVM_INSTALL_INCLUDE_PATH=$LLVM_INSTALL_PATH/include

	# XXX This might go away
	LLVM_LIB_PATH=$LLVM_INSTALL_PATH/lib

	# As we use the installation path, we can use the LLVM config tool. This gives us the correct directory for -L
	# XXX This is deprecated
	LLVM_BIN_PATH=$LLVM_INSTALL_PATH/bin


	AC_MSG_NOTICE(["LLVM install path has been set to $LLVM_INSTALL_PATH"])

	# we list all the libraries we need to link, when we build with clang
	CLANG_LIBS_LIST='-Wl,--start-group,-lclangAST,-lclangAnalysis,-lclangBasic,-lclangDriver,-lclangEdit,-lclangFrontend,-lclangFrontendTool,-lclangLex,-lclangParse,-lclangSema,-lclangEdit,-lclangASTMatchers,-lclangRewrite,-lclangRewriteFrontend,-lclangStaticAnalyzerFrontend,-lclangStaticAnalyzerCheckers,-lclangStaticAnalyzerCore,-lclangSerialization,-lclangToolingCore,-lclangTooling,--end-group'
	
	
	AC_MSG_NOTICE(["Clang Libs: $CLANG_LIBS_LIST"])

	# XXX It might be sufficient to have the source include files.
	AC_SUBST(CLANG_SRC_INCLUDE_PATH)
	AC_SUBST(LLVM_SRC_INCLUDE_PATH)
	AC_SUBST(CLANG_INSTALL_INCLUDE_PATH)
	AC_SUBST(LLVM_INSTALL_INCLUDE_PATH)

	AC_SUBST(CLANG_LIBS_LIST)

	LLVM_CXXFLAGS_FLAGS="\`$LLVM_BIN_PATH/llvm-config --cxxflags\`"
	LLVM_LDFLAGS="\`$LLVM_BIN_PATH/llvm-config --ldflags --libs --system-libs\`"

	AC_SUBST(LLVM_CXXFLAGS_FLAGS)
	AC_SUBST(LLVM_LDFLAGS)
else
	LLVM_INSTALL_PATH="UDEF"
	echo "No llvm installation path set"
fi

if test $LLVM_SRC_PATH != "UDEF" ; then
	if test $LLVM_INSTALL_PATH == "UDEF" ; then
		AC_MSG_ERROR(["Please set llvm-src as well as llvm-install"])
	fi
fi

if test $LLVM_INSTALL_PATH != "UDEF" ; then
	if test $LLVM_SRC_PATH == "UDEF" ; then
		AC_MSG_ERROR(["Please set llvm-src as well as llvm-install"])
	fi
fi

AS_IF([test x"${with_llvm_src}${with_llvm_install}" != xnono],[found_llvm=1], [found_llvm=0])


# -------------------------------
# 4.1) add BOOST
# -------------------------------
AX_BOOST_BASE([1.47])
# JP: This is just output, to see how boost flags look like
AC_MSG_NOTICE(["ac_boost_path=$ac_boost_path"])
AC_MSG_NOTICE(["BOOST_CPPFLAGS=$BOOST_CPPFLAGS"])
AC_MSG_NOTICE(["BOOST_LDFLAGS=$BOOST_LDFLAGS"])
AX_BOOST_IOSTREAMS
AX_BOOST_SYSTEM
AX_BOOST_REGEX
# AX_BOOST_REQUIRE([1.47])
BOOST_INCLUDEDIR="$ac_boost_path/include"
BOOST_LIBDIR="$ac_boost_path/lib"

# -------------------------------
# check for rapidjson
# -------------------------------
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

AM_CONDITIONAL(HAVERAPIDJSON, test x"$with_rapidjson" != xno -a $found_rapidjson = 1)
AC_SUBST(RAPIDJSON_INCLUDEDIR)


AM_CONDITIONAL(USEROSE, test x"$with_rose" != xno)
AM_CONDITIONAL(USECLANG, test x"$with_llvm_src" != xno)

AC_SUBST(CONFIG_DISABLEEXCEPTIONS, [$found_llvm])
AC_SUBST(CONFIG_USE_ROSE, [$found_rose])
AC_SUBST(CONFIG_USE_CLANG, [$found_llvm])
AC_SUBST(CONFIG_INSTALL_DIR, [$prefix])

# -------------------------------
# 4) Checks for libraries.
# -------------------------------

# Export Boost variables
AC_SUBST(BOOST_INCLUDEDIR)
AC_SUBST(BOOST_LIBDIR)


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
