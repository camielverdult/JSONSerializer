cmake_minimum_required(VERSION 3.20)
project(JSONSerializer C CXX)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
FetchContent_MakeAvailable(googletest)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

add_executable(JSONSerializer JSON.h main.c)

# This is all for google test
enable_testing()

add_executable(
        JSONSerializer_gtest
        Tests/JSONTest.cpp
)
target_link_libraries(
        JSONSerializer_gtest
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(JSONSerializer_gtest)
