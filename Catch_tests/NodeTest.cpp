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
  CHECK(ponodes[0]->name == "5");
  CHECK(ponodes[1]->name == "6");
  CHECK(ponodes[2]->name == "3");
  CHECK(ponodes[3]->name == "2");
  CHECK(ponodes[4]->name == "4");
  CHECK(ponodes[5]->name == "1");
};

TEST_CASE("binarise", "[regular]") {
  std::string newick { "((a,b,c,d)e)f" };
  Node* node { parse(std::vector<char>(newick.begin(), newick.end())) };
  node->remove_redundant_nodes();
  node->resolve_polytomies();
  CHECK(node->to_newick() == "(a,(b,(c,d)))e;");
};

TEST_CASE("visit", "[regular]") {
  std::string newick { "(a:1.1,c)b" };
  Node* node { parse(std::vector<char>(newick.begin(), newick.end())) };
  node->visit();
  CHECK(true);
};

TEST_CASE("to_newick", "[regular]") {
  std::string newick { "(a:1.1)b" };
  Node* node { parse(std::vector<char>(newick.begin(), newick.end())) };
  CHECK(node->to_newick() == "(a:1.1)b;");
};

TEST_CASE("print_ascii", "[regular]") {
  std::string newick { "(a)b" };
  Node* node { parse(std::vector<char>(newick.begin(), newick.end())) };
  CHECK(node->ascii_art()[0] == "─b──a");
};

TEST_CASE("print_ascii_2", "[regular]") {
  std::string newick { "(a,b)cc" };
  Node* node { parse(std::vector<char>(newick.begin(), newick.end())) };
  CHECK(node->ascii_art()[0] == "    ┌a");
  CHECK(node->ascii_art()[1] == "─cc─┤");
  CHECK(node->ascii_art()[2] == "    └b");
};

TEST_CASE("print_ascii_3", "[regular]") {
  std::string newick { "(a,b,cc)c" };
  Node* node { parse(std::vector<char>(newick.begin(), newick.end())) };
  std::vector<std::string> lines {node->ascii_art()};
  CHECK(lines[0] == "    ┌a");
  CHECK(lines[1] == "─c──┼b");
  CHECK(lines[2] == "    └cc");
};

TEST_CASE("print_ascii_4", "[regular]") {
  std::string newick { "((x,y)a,b,cc)c" };
  Node* node { parse(std::vector<char>(newick.begin(), newick.end())) };
  std::vector<std::string> lines {node->ascii_art()};
  CHECK(lines[0] == "        ┌x");
  CHECK(lines[1] == "    ┌a──┤");
  CHECK(lines[2] == "    │   └y");
  CHECK(lines[3] == "─c──┤");
  CHECK(lines[4] == "    ├b");
  CHECK(lines[5] == "    └cc");
};
