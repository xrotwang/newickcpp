#include <format>
#include <iostream>
#include <vector>
#include <argparse/argparse.hpp>

#include "parser.h"
#include "newick_lib/util.h"


int main(int argc, char **argv) {
    argparse::ArgumentParser program("newick");
    std::string cmd;
    program.add_argument("cmd").store_into(cmd);
    std::string path;
    program.add_argument("input").store_into(path);
    try {
        program.parse_args(argc, argv);
    } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    Node *tree {parse(read_file(path))};

    if (cmd == "binarise") {
        std::cout << tree->to_newick() << std::endl;
        tree->remove_redundant_nodes();
        tree->resolve_polytomies();  // now we have binary tree!
        std::cout << tree->to_newick() << std::endl;

    } else {
        std::string newick{"((5,6)3,2,4)1"};
        //std::string newick { "((u,v,w,x,y,z)anode:1.45,(((xxx)zz,yyy)xx,yy)bnode:23.4)cnode:2.3" };
        Node *root{parse(std::vector<char>(newick.begin(), newick.end()))};
        root->visit();
        std::vector<Node *> ponodes{root->postorder_traversal()};
        for (unsigned int i = 0; i < ponodes.size(); i++) {
            std::cout << ponodes[i]->name << std::endl;
        }

    }
    return 0;
}
