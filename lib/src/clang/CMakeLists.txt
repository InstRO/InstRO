
add_library(InstRO_clang SHARED ClangInstrumentor.cpp)
set(lib_instro_clang_incl "${CMAKE_SOURCE_DIR}/lib/include/instro/clang")

target_include_directories(InstRO_clang PUBLIC "${CMAKE_SOURCE_DIR}/lib/include")
target_compile_options(InstRO_clang PUBLIC ${instro_default_options})

# Have a short hand.
set(ccsd ${CMAKE_CURRENT_SOURCE_DIR})

# FIXME split that into the actual groups...
set(clang_core_files 	${ccsd}/core/ClangConstruct
											${ccsd}/core/ClangPassFactory
											${ccsd}/pass/adapter/ClangCygProfileAdapter
											${ccsd}/pass/adapter/ClangMangledNameOutputAdapter
											${ccsd}/pass/selector/ClangBlackWhitelistSelector
											${ccsd}/support/ClangConsumerFactory
											${ccsd}/support/InstROASTConsumer
											${ccsd}/tooling/ClangConstructTraitInterface)
											#											${ccsd}/utility/ClangConfigurationPassRegistry)

target_sources(InstRO_clang PUBLIC ${clang_core_files})
target_link_libraries(InstRO_clang PRIVATE ${CLANG_EXPORTED_TARGETS})
