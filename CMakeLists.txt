cmake_minimum_required(VERSION 3.4.3)

project(InstRO)

find_package(LLVM REQUIRED)
find_package(Clang REQUIRED CONFIG)
find_package(Boost REQUIRED COMPONENTS system iostreams program_options)

list(APPEND CMAKE_MODULE_PATH
        ${LLVM_CMAKE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(AddLLVM)
include(llvm-lit)

option(ENABLE_DEBUG_OUTPUT OFF "Enables logging debug messages")
if (ENABLE_DEBUG_OUTPUT)
    add_definitions("-DINSTRO_LOG_DEBUG")
endif()



# TODO I think we can move this to the respective targets
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${LLVM_DEFINITIONS})

# Standard compile options InstRO uses
set(instro_default_options -std=c++14 -Werror=return-type)
include_directories(${CMAKE_BINARY_DIR}/lib/include)

# To select the Compiler backend InstRO wants to use
# TODO As soon as we port the Rose build to CMake, we need to make this configurable
set(INSTRO_USE_CLANG True)
# Whether we can use exceptions
set(INSTRO_DISABLE_EXCEPTIONS False)
# Generate the needed lib/include/instro/config.h file
configure_file(cconfig.h.in ${CMAKE_BINARY_DIR}/lib/include/instro/config.h)

# Process the subsequent directories
add_subdirectory(lib)
add_subdirectory(support)
add_subdirectory(test)
add_subdirectory(examples)
