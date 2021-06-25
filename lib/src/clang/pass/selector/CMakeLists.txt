
add_library(InstRO_clang_selector SHARED ClangBlackWhitelistSelector.cpp)


target_compile_options(InstRO_clang_selector PUBLIC ${instro_default_options})
target_include_directories(InstRO_clang_selector PUBLIC "${CMAKE_SOURCE_DIR}/lib/include")
