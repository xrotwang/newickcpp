#include <cassert>
#include <format>
#include <iostream>
#include <vector>

#include "parser.h"
#include "newick_lib/util.h"


int main(int /*argc*/, char** argv) {
    const std::vector<char>   util {read_file(argv[1])};

    std::cout << argv[1] << std::endl;

    const Node root { parse(util) };
    assert(root.name == "root");

    for (unsigned long i = 0; i < util.size(); i++) {
        switch (util[i]) {
            case '#':
                std::cout << "hash: " << util[i];
                break;
            case '<':
                std::cout << "lt: " << util[i];
                break;
            case '>':
                std::cout << "gt: " << util[i];
                break;
            default:
                std::cout << util[i];
        }

        if (i > 20) {
            break;
        }
    }
    std::cout << std::endl;

    return 0;
}
