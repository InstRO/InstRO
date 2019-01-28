
add_library(InstRO_clang_core SHARED ClangConstruct
																		 ClangPassFactory)
																		
target_compile_options(InstRO_clang_core PUBLIC ${instro_default_options})
target_include_directories(InstRO_clang_core PUBLIC "${CMAKE_SOURCE_DIR}/lib/include")
