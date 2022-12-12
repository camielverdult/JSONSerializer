cmake_minimum_required(VERSION 3.20)
project(JSONSerializer C CXX)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
FetchContent_MakeAvailable(googletest)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

add_executable(JSONSerializer main.c JSON.h JSON.c)

# This is all for google test
enable_testing()

add_executable(
        JSONSerializer_gtest
        JSON.h JSON.c
        Tests/JSONTest.cpp
)
target_link_libraries(
        JSONSerializer_gtest
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(JSONSerializer_gtest)
