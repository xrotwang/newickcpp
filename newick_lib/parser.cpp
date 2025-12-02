#include <algorithm>
#include <cassert>
#include <iostream>
#include <ostream>
#include <utility>
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
        char character = characters[i];
        switch (character) {
            // FIXME: colon, splitting name and branch length!
            case ';':
                //tokens.push_back(Token(character, TokenType::SEMICOLON, level));
                // FIXME: recognize or raise exception
                break;
            case ',':
                tokens.emplace_back(character, TokenType::COMMA, level);
                break;
            case ':':
                tokens.emplace_back(character, TokenType::COLON, level);
                break;
            case '(':  // Braces have the level of the outer scope.
                tokens.emplace_back(character, TokenType::OBRACE, level);
                level++;
                break;
            case ')':
                level--;
                assert(level >= 0);
                tokens.emplace_back(character, TokenType::CBRACE, level);
                break;
            default:
                tokens.emplace_back(character, TokenType::CHAR, level);
        }
    }
};

// We can instantiate NewickString from (subsets of) tokens.
NewickString::NewickString(const std::vector<Token> &tokens_)
    : tokens { std::vector<Token>() }
{
    for (auto token : tokens_) {
        tokens.push_back(token);
    }
    min_level = tokens[0].level;
};

// We can instantiate NewickString from a string.
NewickString::NewickString(const std::string &string)
    : tokens { std::vector<Token>() }
{
    tokens = NewickString(std::vector<char>(string.begin(), string.end())).tokens;
};

Node* NewickString::to_node() {
    std::vector<Token> root_tokens { std::vector<Token>() };

    // Parse root name and length.
    for (unsigned long i = tokens.size(); i > 0; i--) {
        Token token = tokens[i - 1];
        if (token.character == '\0') {
            continue;
        }
        if (token.level != min_level) {
            break;
        }
        if (token.type != TokenType::CBRACE) {
            assert(token.type == TokenType::CHAR || token.type == TokenType::COLON);
            root_tokens.push_back(token);
        }
    }
    std::reverse(root_tokens.begin(), root_tokens.end());

    std::string name { "" };
    std::string length { "" };
    bool in_name { true };
    for (unsigned long i = 0; i < root_tokens.size(); i++) {
        Token token = root_tokens[i];
        if (token.type == TokenType::COLON) {
            in_name = false;
            continue;
        }
        if (in_name) {
            name.append(std::string(1, token.character));
        } else {
            length.append(std::string(1, token.character));
        }
    }

    Node *ntmp;
    ntmp = new Node(name, length);
    std::vector<NewickString> descendants { get_descendants() };
    for (unsigned long i = 0; i < descendants.size(); i++) {
        Node* child = descendants[i].to_node();
        ntmp->add_child(child);
    }
    return ntmp;
};


std::vector<NewickString> NewickString::get_descendants() const {
    std::vector<NewickString> descendants { std::vector<NewickString>() };
    std::vector<Token> descendant_tokens { std::vector<Token>() };
    bool comma { false };

    for (auto token : tokens) {
        if (token.type == TokenType::COMMA && token.level == min_level + 1) {
            comma = true;
            descendants.emplace_back(descendant_tokens);
            descendant_tokens = std::vector<Token>();
        } else if (token.level > min_level) {
            descendant_tokens.emplace_back(token.character, token.type, token.level);
        }
    }
    if (comma || !descendant_tokens.empty()) {
        descendants.emplace_back(descendant_tokens);
    }
    return descendants;
}



int NewickString::get_min_level() const {
    return min_level;
}


Node* parse(std::vector<char> characters) {
    return NewickString(std::move(characters)).to_node();
}
