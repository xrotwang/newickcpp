#include <catch2/catch_test_macros.hpp>

#include "parser.h"


TEST_CASE("NewickString from string", "[category?]"){
    NewickString ns { "(a,b)c" };
    CHECK(ns.get_min_level() == 0);
}

TEST_CASE("NewickString from tokens", "[category?]"){
    NewickString ns { "(a,b)c;" };

    std::vector<Token> tokens { std::vector<Token>() };
    for (auto & token : ns.tokens) {
        if (token.level > 0) {
            tokens.push_back(token);
        }
    }
    CHECK(NewickString(tokens).get_min_level() == 1);
}
