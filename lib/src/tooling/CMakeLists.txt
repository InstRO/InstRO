

add_library(InstRO_tooling  SHARED ConstructElevator
														ControlFlowGraph
														ExtendedCallGraph
														IdentifierProvider
														NamedConstructAccess)

													target_compile_options(InstRO_tooling PUBLIC ${instro_default_options})
													target_include_directories(InstRO_tooling PUBLIC "${CMAKE_SOURCE_DIR}/lib/include" ${Boost_INCLUDE_DIRS})

