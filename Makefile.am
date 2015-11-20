#
# Build the library
#
SUBDIRS= \
	lib \
	test \
	support

#
# Build the examples
#
if BUILD_EXAMPLES
SUBDIRS+= examples
endif

printSetup:
	@echo Rose expected at $(ROSE_INSTALL_DIR)
	@echo BOOST expected at $(BOOST_INSTALL_DIR)
