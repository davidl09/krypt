include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(tests tests.cpp)
target_link_libraries(
        tests database
        server
        Crow::Crow
        gtest_main)
