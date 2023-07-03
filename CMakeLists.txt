cmake_minimum_required(VERSION 3.26.4)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)
set (CMAKE_CXX_EXTENSIONS TRUE)
set (CMAKE_BUILD_TYPE Debug)

# find_package(GTest CONFIG REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

project(btree)

set(SOURCES
    src/btree.cpp
)

add_library(btree src/btree.cpp)

enable_testing()

add_executable(
  btree_test
  tests/btree_test.cpp
)

target_link_libraries(
  btree_test
  GTest::gtest_main
)

target_link_libraries(
  btree_test
  btree
  GTest::gtest_main
)

add_test(test_all btree_test)

