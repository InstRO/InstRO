

add_library(InstRO_pass SHARED adapter/ConstructPrinterAdapter
												adapter/DefaultInstrumentationAdapter
												selector/BooleanCompoundSelector
												selector/CallPathSelector
												selector/ConstructTraitSelector
												selector/ElevatorSelector
												selector/IdentifierMatcherSelector
												selector/ProgramEntrySelector)

											target_compile_options(InstRO_pass PUBLIC ${instro_default_options})
											target_include_directories(InstRO_pass PUBLIC "${CMAKE_SOURCE_DIR}/lib/include")
