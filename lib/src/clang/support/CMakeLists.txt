
add_library(InstRO_clang_support SHARED ClangConsumerFactory
                                                                                                                                        InstROASTConsumer
                                                                                                                                        ClangHelpers)


target_compile_options(InstRO_clang_support PUBLIC ${instro_default_options})
target_include_directories(InstRO_clang_support PUBLIC "${CMAKE_SOURCE_DIR}/lib/include")
