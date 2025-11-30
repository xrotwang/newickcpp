cmake_minimum_required(VERSION 4.0)
project(newick)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion")

include_directories(newick_lib)
add_subdirectory(newick_lib)

add_executable(newick main.cpp
        main.cpp
        newick_lib/util.cpp
        newick_lib/util.h
        newick_lib/node.cpp
        newick_lib/node.h)

target_link_libraries(newick newick_lib)
add_subdirectory(Catch_tests)
