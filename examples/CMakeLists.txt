
add_executable(ClangInstrumentor ClangSrcToSrcFunctionInstrumentor/Instrumentor.cpp)
target_link_libraries(ClangInstrumentor InstRO)
target_include_directories(ClangInstrumentor PUBLIC "${CMAKE_SOURCE_DIR}/lib/include")
target_include_directories(ClangInstrumentor PUBLIC ${LLVM_INCLUDE_DIRS})


add_subdirectory(targets)
add_subdirectory(TestProject)
