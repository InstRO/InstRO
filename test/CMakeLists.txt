
# Build the supporting infrastructure library for the tests
add_library(InstRO_clang_test lib/TestAdapter.cpp)

target_include_directories(InstRO_clang_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/lib/include)
target_compile_options(InstRO_clang_test PUBLIC ${instro_default_options})

# Build the test case binary
add_executable(ConstructHierarchySelectionTest ConstructHierarchySelectionTestMain)

string(REGEX REPLACE "clang-format|clang-import-test|clang-rename|clang-refactor|clang-reorder-fields|modularize|clang-tidy|clangd" "" CLANG_EXPORTED_TARGETS "${CLANG_EXPORTED_TARGETS}")

target_include_directories(ConstructHierarchySelectionTest PUBLIC "${CMAKE_SOURCE_DIR}/lib/include" ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
target_compile_options(ConstructHierarchySelectionTest PUBLIC ${instro_default_options})
target_link_libraries(ConstructHierarchySelectionTest InstRO_clang_test InstRO ${CLANG_EXPORTED_TARGETS} LLVMSupport ${Boost_LIBRARIES})
