cmake_minimum_required(VERSION 3.5...10.0)
project(utilities LANGUAGES CXX)

# google mock libs
include_directories($ENV{GMOCK_HOME}/include $ENV{GMOCK_HOME}/gtest/include)
link_directories($ENV{GMOCK_HOME}/mybuild $ENV{GMOCK_HOME}/gtest/mybuild)

# standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# flags
set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS}")# -fmodules-ts -Wall -Wextra")

# sources
set(sf_sources src/run_sanitise_filename.cpp src/sanitise_filename.cpp)
FILE(GLOB test_sources src/test*.cpp)

# targets
add_executable(sanitise_filename ${sf_sources})
add_executable(test ${test_sources} src/sanitise_filename.cpp)

# link
target_link_libraries(sanitise_filename boost_regex fmt)
target_link_libraries(test boost_regex fmt)
target_link_libraries(test pthread)
target_link_libraries(test gmock)
target_link_libraries(test gtest)
