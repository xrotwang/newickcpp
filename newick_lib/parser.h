#ifndef NEWICK_PARSER_H
#define NEWICK_PARSER_H

#include <memory>
#include <vector>
#include "node.h"

enum TokenType {
    CHAR = 1,
    QWORD = 2,  // A quoted string
    COMMENT = 3,
    WHITESPACE = 4,
    OBRACE = 5,  // Opening brace
    CBRACE = 6,  // Closing brace
    COMMA = 7,
    COLON = 8,
    SEMICOLON = 9
};

class Token {
public:
    char character;
    TokenType type;
    int level;
    explicit Token(char character, TokenType type, int level);
};

class NewickString {
    int min_level { 0 };
public:
    std::vector<Token> tokens;

    explicit NewickString(std::vector<char> characters);
    explicit NewickString(const std::vector<Token> &tokens);
    explicit NewickString(const std::string &string);
    [[nodiscard]] std::unique_ptr<Node> to_node() const;
    [[nodiscard]] int get_min_level() const;
    [[nodiscard]] std::vector<NewickString> get_descendants() const;
};

std::unique_ptr<Node> parse(std::vector<char> characters);

#endif //NEWICK_PARSER_H
