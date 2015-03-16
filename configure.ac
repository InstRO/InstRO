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
# CI: not available until autoconf 1.14
# AC_CONFIG_MACRO_DIRS([config/m4])
AM_INIT_AUTOMAKE([foreign subdir-objects])
PKG_PROG_PKG_CONFIG


# -------------------------------
# 1.3 config.h
# -------------------------------
# As Automake prefers its version, let automake generate the header
AC_CONFIG_SRCDIR([config.h.in])
# AC_CONFIG_HEADERS([config.h])
AM_CONFIG_HEADER([config.h])
## CI: old version AC_CONFIG_HEADERS([config.h])


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


# 
# 1.6 Set AC_PROG_INSTALL
AC_PROG_INSTALL

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
             yes) debug=true ;;
             no)  debug=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[debug=false])

AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")


# -------------------------------
# 2.0.1) Add examples option
# -------------------------------

#AC_ARG_ENABLE([examples], 
#	AS_HELP_STRING([--disable-examples], [disable examples default: yes?]),
#	 [case "${enableval}" in 
#						yes)	examples=true ;;
#						no)		examples=false ;;
#			esac],[examples=false])
#
AM_CONDITIONAL(BUILD_EXAMPLES, [test x$examples = xtrue])

# ------------------------------
# 2.0.2) Add publication tools
# ------------------------------

#AC_ARG_ENABLE([publication-tools],
#	AS_HELP_STRING([--disable-publication-tools], [disable building
#									tools that were used in publications]),
#		[case "${enableval}" in
#						yes)	publications=true ;;
#						no)		publications=false ;;
#		esac],[publications=true])
#
AM_CONDITIONAL(BUILD_PUBLICATIONS_TOOLS, [test x$publications = xtrue])

# -------------------------------
# 2.1) add ROSE
# -------------------------------
# introduce the optional configure parameter for a non-standard install prefix of ROSE
AC_ARG_WITH([ROSE],[AS_HELP_STRING([--with-ROSE=prefix],
    [try this for a non-standard install prefix of the ROSE library])],
    [ROSEPATHSET=1],[ROSEPATHSET=0])

AC_ARG_WITH([ROSE_INCLUDE_PATH],[AS_HELP_STRING([--with-ROSE_include_path=prefix],
    [try this for a non-standard include path of the ROSE library])],
    [ROSE_INC_PATHSET=1],[ROSE_INC_PATHSET=0])
    
# if optional parameter used, get the path for the desired rose directory (and library directory)
if test $ROSEPATHSET = 1 ; then
    # extend the compiler and linker flags according to the path set
    ROSE_INCLUDEDIR="$with_ROSE/include"
    ROSE_NEW_INCLUDEDIR="$with_ROSE/include/rose"
    ROSE_LIBDIR="$with_ROSE/lib"
    echo "Using ROSE in $with_ROSE"
else
    ROSE_INCLUDEDIR=/usr/include
    ROSE_NEW_INCLUDEDIR=/usr/include/rose
    ROSE_LIBDIR=/usr/lib
fi

## set @ROSE_INCLUDEDIR@ and @ROSE_LIBDIR@ variables
AC_SUBST(ROSE_INCLUDEDIR)
AC_SUBST(ROSE_NEW_INCLUDEDIR)
AC_SUBST(ROSE_LIBDIR)

# ---------------------------
# 2.2) Add Clang flag
# ---------------------------
AC_ARG_WITH([clang], [AS_HELP_STRING([--with-clang=prefix], 
						[Tells the build process the toplevel directory of the clang installation])], 
						[CLANGPATHSET=1],[CLANGPATHSET=0])
if test $CLANGPATHSET = 1 ; then
	CLANG_BASE_PATH="$with_clang"
	CLANG_INCLUDE_PATH=$CLANG_BASE_PATH/include
	CLANG_LIB_DIR=$CLANG_BASE_PATH/lib
	LLVM_INCLUDE_PATH=$CLANG_BASE_PATH/include
# in order to query the tools for the compile flags we need to know the binary dir
	CLANG_BIN_SUPPORT=$CLANG_BASE_PATH/bin
	AC_MSG_NOTICE(["Clang base path has been set to $CLANG_BASE_PATH"])
	AC_MSG_NOTICE(["Clang include path has been set to $CLANG_INCLUDE_PATH"])

	# we list all the libraries we need to link, when we build with clang
	CLANG_LIBS_LIST='-Wl,--start-group,-lclangAST,-lclangAnalysis,-lclangBasic,-lclangDriver,-lclangEdit,-lclangFrontend,-lclangFrontendTool,-lclangLex,-lclangParse,-lclangSema,-lclangEdit,-lclangASTMatchers,-lclangRewrite,-lclangRewriteFrontend,-lclangStaticAnalyzerFrontend,-lclangStaticAnalyzerCheckers,-lclangStaticAnalyzerCore,-lclangSerialization,-lclangToolingCore,-lclangTooling,-lclangToolingCore,--end-group'

	AC_MSG_NOTICE(["Clang Libs: $CLANG_LIBS_LIST"])
	# if the clang path is set, then we export those variables
	AC_SUBST(CLANG_INCLUDE_PATH)
	AC_SUBST(LLVM_INCLUDE_PATH)
	# This is ONLY Clang
	AC_SUBST(CLANG_BIN_SUPPORT)
	AC_SUBST(CLANG_LIB_DIR)
	AC_SUBST(CLANG_LIBS_LIST)
	# We also need LLVM
	LLVM_INCLUDE_FLAGS="\`$CLANG_BIN_SUPPORT/llvm-config --cxxflags\`"
	LLVM_LINK_COMMAND="\`$CLANG_BIN_SUPPORT/llvm-config --ldflags --libs --system-libs\`"
	AC_SUBST(LLVM_INCLUDE_FLAGS)
	AC_SUBST(LLVM_LINK_COMMAND)
else
	echo "No clang path set"
fi


# -------------------------------
# 4.1) add BOOST
# -------------------------------
AX_BOOST_BASE([1.47])
# JP: This is just output, to see how boost flags look like
AC_MSG_NOTICE(["ac_boost_path=$ac_boost_path"])
AC_MSG_NOTICE(["CPPFLAGS=$BOOST_CPPFLAGS"])
AC_MSG_NOTICE(["LDFLAGS=$BOOST_LDFLAGS"])
AX_BOOST_IOSTREAMS
AX_BOOST_SYSTEM
AX_BOOST_REGEX
# AX_BOOST_REQUIRE([1.47])
BOOST_INCLUDEDIR="$ac_boost_path/include"
BOOST_LIBDIR="$ac_boost_path/lib"


# ------ enable libtool support
AC_ARG_ENABLE(libtool,
AS_HELP_STRING([--enable-libtool],
               [enable use of libtool, default: please no]),  
[case "${enableval}" in
             yes) libtool=true ;;
             no)  libtool=false ;;
             *)    AC_MSG_ERROR([bad value ${enableval} for --enable-libtool]) ;;
esac],
[libtool=false])

AM_CONDITIONAL(USELIBTOOL, test x"$libtool" = x"true")


# -------------------------------
# 3) Checks for programs.
# -------------------------------
AC_PROG_CXX
AC_PROG_CC
# AC_PROG_INSTALL
AC_PROG_MAKE_SET
# LT_INIT


# -------------------------------
# 4) Checks for libraries.
# -------------------------------
# CI: Check for BOOST
##    S.th. messes up, disabling boost tests for starters
#AX_BOOST_THREAD
#AX_BOOST_DATE_TIME
#AX_BOOST_REGEX
#AX_BOOST_PROGRAM_OPTIONS
##AX_BOOST_SERIALIZATION
##AX_BOOST_ASIO
##AX_BOOST_SIGNALS
##AX_BOOST_TEST_EXEC_MONITOR
#AX_BOOST_SYSTEM
#AX_BOOST_FILESYSTEM
#AX_BOOST_WAVE


# CI: Check for ROSE
AC_CHECK_LIB([rose], [frontend])


##AC_DEFINE([BOOST_INCLUDEDIR], [$BOOST_INCLUDEDIR], [Description])
##AC_SUBST(BOOST_INCLUDEDIR,[$BOOST_INCLUDEDIR])
##AC_SUBST(BOOST_LIBDIR,[$BOOST_LIBDIR])

# AM_SUBST_NOTMAKE(BOOST_INCLUDEDIR)
# AM_SUBST_NOTMAKE(BOOST_LIBDIR)
AC_SUBST(BOOST_INCLUDEDIR)
AC_SUBST(BOOST_LIBDIR)
AC_SUBST(LDFLAGS)





# -------------------------------
# 4.9) Set Variables.
# -------------------------------

ROSE_CPPFLAGS="$BOOST_CPPFLAGS -I$ROSE_INCLUDEDIR -I$ROSE_INCLUDEDIR/rose"
# JP: I am renaming this, so using this option is deprecated
#ROSE_LDFLAGS="-lrose $BOOST_IOSTREAMS_LIB $BOOST_SYSTEM_LIB"
# JP: This is the new one we use now!
ROSE_LINK_LIBRARIES="-lrose $BOOST_IOSTREAMS_LIB $BOOST_SYSTEM_LIB $BOOST_REGEX_LIB"

INSTRO_CPP_FLAGS="$INSTRO_CPP_FLAGS --version "

AC_MSG_NOTICE(["INSTRO_CXXFLAGS= $INSTRO_CXXFLAGS"])
AC_MSG_NOTICE(["INSTRO_CPPFLAGS= $INSTRO_CPPFLAGS"])
AC_MSG_NOTICE(["INSTRO_LDFLAGS= $INSTRO_LDFLAGS"])

AC_SUBST(INSTRO_CPPFLAGS)
AC_SUBST(INSTRO_CFLAGS)
AC_SUBST(INSTRO_CXXFLAGS)
AC_SUBST(INSTRO_LDFLAGS) 
AC_SUBST(ROSE_CPPFLAGS)
AC_SUBST(ROSE_CFLAGS)
AC_SUBST(ROSE_CXXFLAGS)
# JP: I guess this has to go away then...DIE!!!!
#AC_SUBST(ROSE_LDFLAGS)
# JP: This is the new one we use!
AC_SUBST(ROSE_LINK_LIBRARIES)

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

## CI: add makefiles here for configure to install in build directory
#AC_CONFIG_FILES([Makefile
#                 examples/BlackWhitelistSelector/Makefile
#                 examples/CygProfileInstrumentor/Makefile
#                 examples/Geimer/Makefile
#                 examples/GenericPrinterExample/Makefile
#                 examples/Makefile
#                 lib/Makefile])
AC_CONFIG_FILES([Makefile
		 lib/Makefile
		 ])

#AM_COND_IF([BUILD_PUBLICATIONS_TOOLS], [
#	AC_CONFIG_FILES([
#		 examples/Instrumentors/Makefile
#	])])

#AM_COND_IF([BUILD_EXAMPLES],[
#	AC_CONFIG_FILES([
#		 examples/BlackWhitelistSelector/Makefile
#		 examples/ExcludeFilePathInstrumentor/Makefile
#		 examples/FunctionPrinter/Makefile
#		 examples/CallSiteInstrumentor/Makefile
#		 examples/CallGraphPrinter/Makefile
#		 examples/CygProfileInstrumentor/Makefile
#		 examples/ProfileBasedSelector/Makefile
#		 examples/MPIInstrumentor/Makefile
#		 examples/IdentityTranslator/Makefile
#		 examples/NameMatchingSelector/Makefile
#		 examples/CallFrequencyHeuristic/Makefile
#		 examples/CompilerSupportedSamplingInstrumentor/Makefile
#		 examples/CallPathInstrumentor/Makefile
#		 examples/AdvancedAnalyticsInstrumentor/Makefile
#		])])

# -------------------------------
# 8) generate Makefiles from Makefil.in
# -------------------------------
AC_OUTPUT
