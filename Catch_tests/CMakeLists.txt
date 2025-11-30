find_package(Catch2 3 REQUIRED)
add_executable(Catch_tests_run NodeTest.cpp
        NewickStringTest.cpp)
target_link_libraries(Catch_tests_run PRIVATE newick_lib)
target_link_libraries(Catch_tests_run PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(Catch_tests_run)
