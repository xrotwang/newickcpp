//
// Created by robert on 11/29/25.
//
#include <catch2/catch_test_macros.hpp>

#include "util.h"

TEST_CASE("dunno", "[category?]"){  // 12/2/2020 -> 737761
    CHECK(mult(4, 5) == 20);
}

TEST_CASE("dunno2", "[category?]"){  // 12/2/2020 -> 737761
    CHECK(mult(4, 6) == 24);
}

TEST_CASE("read_file_test", "[catefory?]") {
    CHECK(read_file("NodeTest.cpp")[0] == '/');
}
