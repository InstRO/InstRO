
# Process the different part-libraries
add_subdirectory(core)
add_subdirectory(pass/adapter)
add_subdirectory(pass/selector)
add_subdirectory(support)
add_subdirectory(tooling)
# This is part of the todo 
#add_subdirectory(utility)

# To build the clang abstraction layer implementation

add_library(InstRO_clang SHARED ClangInstrumentor.cpp)

target_include_directories(InstRO_clang PUBLIC "${CMAKE_SOURCE_DIR}/lib/include")
target_compile_options(InstRO_clang PUBLIC ${instro_default_options})

# Have a short hand.
set(ccsd ${CMAKE_CURRENT_SOURCE_DIR})

# TODO Include the Rapidjson library
# ${ccsd}/utility/ClangConfigurationPassRegistry)

message("Clang Core: ${CLANG_EXPORTED_TARGETS}")

target_sources(InstRO_clang PUBLIC ${clang_core_files})
target_link_libraries(InstRO_clang
		PUBLIC
		InstRO_clang_core
		InstRO_clang_adapter
		InstRO_clang_selector
		InstRO_clang_support
		InstRO_clang_tooling
		clangAnalysis
		clangAST
		clangASTMatchers
		clangBasic
		clangFormat
		clangFrontend
		clangLex
		clangRewrite
		clangSema
		clangSerialization
		clangTooling
		clangToolingCore
																					#[[${CLANG_EXPORTED_TARGETS}]])
