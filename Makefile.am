#
# For necessary flags we use the naming USEXXX
# For optional components we use the naming WITHYYY
#

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

# Runs a showcase. Note that it will build InstRO if it wasn't already built. However, it will use only one process to do so.
example-run:
	make -C $(top_builddir)
	$(top_builddir)/examples/RoseCallpathToKernelInstrumentor $(top_srcdir)/examples/targets/MatrixMultiply.cpp

endif

printSetup:
	@echo Rose expected at $(ROSE_INSTALL_DIR)
	@echo BOOST expected at $(BOOST_INSTALL_DIR)
