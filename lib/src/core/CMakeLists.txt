

add_library(InstRO_core 	SHARED ConstructSet.cpp
														ConstructTraitType.cpp
														PassImplementation.cpp
														SimplePassManager.cpp
														Singleton.cpp)

target_include_directories(InstRO_core PUBLIC "${CMAKE_SOURCE_DIR}/lib/include")
target_compile_options(InstRO_core PUBLIC ${instro_default_options})
