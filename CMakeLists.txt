cmake_minimum_required(VERSION 3.10)
project(RecursiveAutonomousSystem VERSION 4.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -pthread")

add_executable(recursive_system src/consolidated.cpp)
target_link_libraries(recursive_system pthread m)
