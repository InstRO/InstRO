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

example-run:
	$(top_builddir)/examples/RoseCallpathToKernelInstrumentor $(top_srcdir)/examples/targets/MatrixMultiply.cpp

endif

printSetup:
	@echo Rose expected at $(ROSE_INSTALL_DIR)
	@echo BOOST expected at $(BOOST_INSTALL_DIR)
