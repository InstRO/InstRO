# Process part-libraries
add_subdirectory(src/clang)
add_subdirectory(src/core)
add_subdirectory(src/pass)
add_subdirectory(src/tooling)
add_subdirectory(src/utility)

message(STATUS "Clang to use: " ${Clang_DIR})

# The InstRO library
add_library(InstRO SHARED src/Instrumentor 
											src/Pass 
											src/PassFactory)

target_include_directories(InstRO PUBLIC "${CMAKE_SOURCE_DIR}/lib/include")
target_compile_options(InstRO PUBLIC ${instro_default_options})

target_link_libraries(InstRO PUBLIC InstRO_core 
																		InstRO_clang 
																		InstRO_pass 
																		InstRO_util 
																		InstRO_tooling 
																		${Boost_LIBRARIES})
