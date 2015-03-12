# Deprecated as information from Make.def is set in the configure: include Make.def
## CI: deprecated ACLOCAL_AMFLAGS= -I ./config/m4



## CI: Currently only build the library, the examples will follow later once I figured out hot to create dependencies
#SUBDIRS=lib examples/CygProfileInstrumentor  examples/ExcludeFilePathInstrumentor examples/FunctionPrinter examples/CallGraphPrinter examples/BlackWhitelistSelector examples/ProfileBasedSelector examples/MPIInstrumentor examples/IdentityTranslator examples/CallFrequencyHeuristic examples/CompilerSupportedSamplingInstrumentor examples/AdvancedAnalyticsInstrumentor

# CI: initially without examples
# #SUBDIRS= \
# #	lib \
# #	examples

SUBDIRS= \
	lib 

#help:
#	@echo To build and use InstRO
#	@echo first update the Make.def file with the paths to 
#	@echo a\) ROSE
#	@echo b\) BOOST
#	@echo Please also provide an installation path.
#	@echo You may then compile InstRO by either calling make library or by 
#	@echo entering the library directory lib and building there.
#	@echo The libraries can be installed by calling \"make install\" which
#	@echo will install instro to the specified directory

#lib/libinstro.a:
#library:
#	cd ./lib;make

#install:
#	cd ./lib;make install
	


	
printSetup:
	@echo Rose expected at $(ROSE_INSTALL_DIR)
	@echo BOOST expected at $(BOOST_INSTALL_DIR)
