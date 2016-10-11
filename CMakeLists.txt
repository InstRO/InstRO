cmake_minimum_required(VERSION 3.4.3)

project(InstRO)

find_package(Clang REQUIRED CONFIG)

find_package(Boost REQUIRED COMPONENTS system iostreams program_options)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${LLVM_DEFINITIONS})

set(instro_default_options -std=c++14 -Werror=return-type)


set(INSTRO_USE_CLANG True)
set(INSTRO_DISABLE_EXCEPTIONS False)
configure_file(cconfig.h.in ${CMAKE_SOURCE_DIR}/lib/include/instro/config.h)

add_subdirectory(lib)
add_subdirectory(support)
add_subdirectory(test)
add_subdirectory(examples)
