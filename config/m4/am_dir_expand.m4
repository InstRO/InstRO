#
# Fetches auxiliary information which is used within
# the config.h header. Using this information InstRO fetches
# it's build directory.
#

AC_DEFUN([AM_DIR_EXPAND],
	[AC_REQUIRE([AC_CONFIG_AUX_DIR_DEFAULT]) 
	am_aux_dir=`cd "$ac_aux_dir" && pwd`
	am_build_dir=`pwd`
])
