cmake_minimum_required(VERSION 3.20)

project(ammf)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Global include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

# Compile src
add_executable(example src/example.cpp)

# Compile tests
find_package(Catch2 3)
if (Catch2_FOUND)
  message(STATUS "Catch2 found, building tests!")

  add_executable(benchmark test/benchmark.cpp)
  target_link_libraries(benchmark PUBLIC Catch2::Catch2WithMain)
else()
  message(STATUS "Catch2 not found, skipping tests!")
endif()
