cmake_minimum_required(VERSION 3.0)
project(aosoa)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
else()
  set(CMAKE_BUILD_TYPE Debug)
endif()

message("Build type: ${CMAKE_BUILD_TYPE}")

# Good practices
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wshadow-all")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wconversion")

# Enable all instruction subsets supported by the local machine
# TODO: Create  flags for specific instructions sets
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

include_directories(
    # Include files
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

add_executable(benchmark
    # Source files
    src/benchmark.cpp
    )

add_executable(named_tuple_demo
    # Source files
    src/named_tuple_demo.cpp
    )

add_executable(aosoa_demo
    # Source files
    src/aosoa_demo.cpp
    )
