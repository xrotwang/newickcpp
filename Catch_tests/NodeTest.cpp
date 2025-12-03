//
// Created by robert on 11/29/25.
//
#include <catch2/catch_test_macros.hpp>

#include "util.h"
#include "parser.h"
#include "node.h"



TEST_CASE("read_file_test", "[regular]") {
    CHECK(read_file("NodeTest.cpp")[0] == '/');
}


/*
 * postorder test:
 *
Input:
     1
   / | \
  3  2  4
 / \
5   6
Output: [5, 6, 3, 2, 4, 1]
 */
TEST_CASE("postorder", "[regular]") {
 std::string newick { "((5,6)3,2,4)1" };
 Node* node { parse(std::vector<char>(newick.begin(), newick.end())) };
 std::vector<Node*> ponodes { node->postorder_traversal() };
 for (unsigned int i = 0; i < ponodes.size(); i++) {
   if (i == 0)
     CHECK(ponodes[i]->name == "5");
   if (i == 1)
     CHECK(ponodes[i]->name == "6");
   if (i == 2)
     CHECK(ponodes[i]->name == "3");
   if (i == 3)
     CHECK(ponodes[i]->name == "2");
   if (i == 4)
     CHECK(ponodes[i]->name == "4");
   if (i == 5)
     CHECK(ponodes[i]->name == "1");
 }
};
