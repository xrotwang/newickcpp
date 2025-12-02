#include <cassert>
#include <format>
#include <iostream>
#include <vector>

#include "parser.h"
#include "newick_lib/util.h"


int main(int /*argc*/, char** argv) {
    const std::vector<char>   util {read_file(argv[1])};

    std::cout << argv[1] << std::endl;

    std::string newick { "((u,v,w,x,y,z)anode:1.45,(((xxx)zz,yyy)xx,yy)bnode:23.4)cnode:2.3" };
    Node* root { parse(std::vector<char>(newick.begin(), newick.end())) };
    root->visit();
    root->remove_redundant_nodes();
    root->visit();
    root->resolve_polytomies();  // now we have binary tree!
    root->visit();
    return 0;
}
