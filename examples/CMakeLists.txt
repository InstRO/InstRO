
add_executable(ClangInstrumentor ClangSrcToSrcFunctionInstrumentor/Instrumentor.cpp)
target_link_libraries(ClangInstrumentor InstRO ${Boost_LIBRARIES})

add_subdirectory(targets)
add_subdirectory(TestProject)
