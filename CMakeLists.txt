cmake_minimum_required(VERSION 3.29)
project(crypt_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(submodules)
include(FetchContent)
FetchContent_Declare(
  sqlpp11
  GIT_REPOSITORY https://github.com/rbock/sqlpp11
  GIT_TAG main  
)

FetchContent_MakeAvailable(sqlpp11)

add_subdirectory(src)

option(BUILD_TESTS "Build the test suite" ON)

add_subdirectory(tests)
#if(BUILD_TESTS)
#    add_subdirectory(tests)
#endif()