#include <cassert>
#include <format>
#include <iostream>
#include <vector>

#include "parser.h"
#include "newick_lib/util.h"


int main(int /*argc*/, char** argv) {
    const std::vector<char>   util {read_file(argv[1])};

    std::cout << argv[1] << std::endl;

    std::string newick { "(anode:1.45,bnode:23.4)cnode:2.3" };
    parse(std::vector<char>(newick.begin(), newick.end()));

    return 0;
}
