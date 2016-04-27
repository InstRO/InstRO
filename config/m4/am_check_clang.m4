#
# Checks for necessary Clang libraries and LLVM parts
#

AC_DEFUN([AM_CHECK_CLANG],
[
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
#	LLVM_LIB_PATH=$LLVM_INSTALL_PATH/lib

	# As we use the installation path, we can use the LLVM config tool. This gives us the correct directory for -L
	# XXX This is deprecated
	LLVM_BIN_PATH=$LLVM_INSTALL_PATH/bin

	# we list all the libraries we need to link, when we build with clang
	CLANG_LIBS_LIST='-Wl,--start-group,-lclangAST,-lclangAnalysis,-lclangBasic,-lclangDriver,-lclangEdit,-lclangFrontend,-lclangFrontendTool,-lclangLex,-lclangParse,-lclangSema,-lclangEdit,-lclangASTMatchers,-lclangRewrite,-lclangRewriteFrontend,-lclangStaticAnalyzerFrontend,-lclangStaticAnalyzerCheckers,-lclangStaticAnalyzerCore,-lclangSerialization,-lclangToolingCore,-lclangTooling,--end-group'
	

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
])

