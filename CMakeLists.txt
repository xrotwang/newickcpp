cmake_minimum_required(VERSION 3.20)
project(newick)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion")

add_executable(newick)
target_sources(newick
        PRIVATE
        main.cpp
)

include_directories(newick_lib)
add_subdirectory(newick_lib)
target_link_libraries(newick
        PRIVATE
        newick_lib)

add_subdirectory(Catch_tests)