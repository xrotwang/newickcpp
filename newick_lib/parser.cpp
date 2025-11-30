#include <cassert>
#include <iostream>
#include <ostream>
#include <vector>

#include "node.h"
#include "parser.h"


Token::Token(char character, TokenType type, int level)
    : character {character}, type {type}, level {level}
{
};

NewickString::NewickString(std::vector<char> characters)
    : tokens { std::vector<Token>()}
{
    int level { 0 };
    min_level = 0;
    for (unsigned long i = 0; i < characters.size(); i++) {
        switch (characters[i]) {
            // FIXME: colon, splitting name and branch length!
            case ';':
                //tokens.push_back(Token(characters[i], TokenType::SEMICOLON, level));
                // FIXME: recognize or raise exception
                break;
            case ',':
                tokens.push_back(Token(characters[i], TokenType::COMMA, level));
                break;
            case ':':
                tokens.push_back(Token(characters[i], TokenType::COLON, level));
                break;
            case '(':  // Braces have the level of the outer scope.
                tokens.push_back(Token(characters[i], TokenType::OBRACE, level));
                level++;
                break;
            case ')':
                level--;
                assert(level >= 0);
                tokens.push_back(Token(characters[i], TokenType::CBRACE, level));
                break;
            default:
                tokens.push_back(Token(characters[i], TokenType::CHAR, level));
        }
    }
};

// We can instantiate NewickString from (subsets of) tokens.
NewickString::NewickString(std::vector<Token> tokens)
    : tokens {tokens}
{
    min_level = tokens[0].level;
};

// We can instantiate NewickString from a string.
NewickString::NewickString(const std::string &string)
    : tokens { std::vector<Token>() }
{
    tokens = NewickString(std::vector<char>(string.begin(), string.end())).tokens;
};

Node NewickString::to_node() {
    return Node("root", 0, nullptr);
};

int NewickString::get_min_level() {
    return min_level;
}


Node parse(std::vector<char> characters) {
    NewickString ns {characters};
    int semicolons { 0 };
    for (unsigned long i = 0; i < ns.tokens.size(); i++) {
        if (ns.tokens[i].type == TokenType::COMMA) {
            semicolons++;
        }
    }
    std::cout << semicolons << std::endl;
    ns = NewickString(ns.tokens);
    for (unsigned long i = 0; i < ns.tokens.size(); i++) {
        if (ns.tokens[i].type == TokenType::COMMA) {
            semicolons++;
        }
    }
    std::cout << semicolons << std::endl;
    return Node {"root", 0, nullptr};
}
