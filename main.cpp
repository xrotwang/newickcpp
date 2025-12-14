#include <format>
#include <memory>
#include <iostream>
#include <vector>

#include "parser.h"
#include "newick_lib/argparse.hpp"
#include "newick_lib/util.h"

enum Cmd {
    binarise, // 0
    print_ascii, // 1
    help,
};

constexpr Cmd getCmd(const std::string_view sv) {
    if (sv == "binarise") return binarise;
    if (sv == "print-ascii") return print_ascii;
    return help;
}

int main(int argc, char **argv) {
    argparse::ArgumentParser program("newick");
    std::string cmd;
    program.add_argument("cmd")
            .help("{binarise, print-ascii}")
            .choices("binarise", "print-ascii")
            .store_into(cmd);
    std::string path;
    program.add_argument("-f")
            .help("read input from file")
            .default_value("").store_into(path);
    std::string string;
    program.add_argument("-s")
            .help("read input from string argument")
            .default_value("").store_into(string);

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }
    // Read input from file, cli arg or stdin.
    std::vector<char> input;
    if (!path.empty()) {
        input = read_file(path);
    } else if (!string.empty()) {
        input = std::vector<char>(string.begin(), string.end());
    } else {  // read from stdin
        std::string input_line;
        std::getline(std::cin, input_line);
        input = std::vector<char>(input_line.begin(), input_line.end());
    }
    std::unique_ptr<Node> tree {parse(input)};

    switch (getCmd(cmd)) {
        case binarise:
            tree->remove_redundant_nodes();
            tree->resolve_polytomies(); // now we have a binary tree!
            std::cout << tree->to_newick() << std::endl;
            break;
        case print_ascii:
            for (const auto &line: tree->ascii_art()) {
                std::cout << line << std::endl;
            };
            break;
        default:
            // print help
            break;
    }
    return 0;
}
