

add_library(InstRO_clang_test lib/TestAdapter.cpp)
target_include_directories(InstRO_clang_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/lib/include)
target_compile_options(InstRO_clang_test PUBLIC ${instro_default_options})

add_executable(ConstructHierarchySelectionTest ConstructHierarchySelectionTestMain)
target_include_directories(ConstructHierarchySelectionTest PUBLIC "${CMAKE_SOURCE_DIR}/lib/include" ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
target_compile_options(ConstructHierarchySelectionTest PUBLIC -std=c++11 -Werror=return-type)
#target_compile_definitions(ConstructHierarchySelectionTest PUBLIC INSTRO_USE_CLANG)
target_link_libraries(ConstructHierarchySelectionTest InstRO_clang_test InstRO InstRO_clang_test ${CLANG_EXPORTED_TARGETS} ${Boost_LIBRARIES})
