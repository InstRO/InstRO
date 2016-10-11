

add_library(InstRO_util SHARED BWLFileReader
														CommandlineHandling
														#ConfigurationLoader
														Environment
														Logger)

													target_include_directories(InstRO_util PUBLIC ${CMAKE_SOURCE_DIR}/lib/include ${Boost_INCLUDE_DIR})
target_compile_options(InstRO_util PUBLIC ${instro_default_options})
target_link_libraries(InstRO_util PUBLIC ${Boost_LIBRARIES})

