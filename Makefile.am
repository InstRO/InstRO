#
# Build the library
#

SUBDIRS= \
	lib \
	support

#
# Build the examples
#
if BUILD_EXAMPLES
SUBDIRS+= examples
endif

#
# Build the tests
#
# SUBDIRS+= \
	# test


printSetup:
	@echo Rose expected at $(ROSE_INSTALL_DIR)
	@echo BOOST expected at $(BOOST_INSTALL_DIR)
