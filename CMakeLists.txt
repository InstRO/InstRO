cmake_minimum_required(VERSION 3.5)

project(InstRO)

find_package(Clang REQUIRED CONFIG)
find_package(LLVM REQUIRED CONFIG)
find_package(Boost REQUIRED COMPONENTS system iostreams program_options)

# TODO I think we can move this to the respective targets
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${LLVM_DEFINITIONS})

# Standard compile options InstRO uses
set(instro_default_options -std=c++14 -Werror=return-type)
include_directories(${CMAKE_BINARY_DIR}/lib/include)
# FIXME: Make this configurable.
INCLUDE_DIRECTORIES(/opt/modules/sw_repo/boost/1.64/include)

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
