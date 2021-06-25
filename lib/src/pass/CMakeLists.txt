

add_library(InstRO_pass SHARED adapter/ConstructPrinterAdapter.cpp
												adapter/DefaultInstrumentationAdapter.cpp
												selector/BooleanCompoundSelector.cpp
												selector/CallPathSelector.cpp
												selector/ConstructTraitSelector.cpp
												selector/ElevatorSelector.cpp
												selector/IdentifierMatcherSelector.cpp
												selector/ProgramEntrySelector.cpp)

target_compile_options(InstRO_pass PUBLIC ${instro_default_options})
target_include_directories(InstRO_pass PUBLIC "${CMAKE_SOURCE_DIR}/lib/include")
