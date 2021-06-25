
add_library(InstRO_clang_adapter SHARED ClangCygProfileAdapter.cpp
																	ClangMangledNameOutputAdapter.cpp)


target_compile_options(InstRO_clang_adapter PUBLIC ${instro_default_options})
target_include_directories(InstRO_clang_adapter PUBLIC "${CMAKE_SOURCE_DIR}/lib/include")
