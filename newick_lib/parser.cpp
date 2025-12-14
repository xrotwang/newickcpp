#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

#include "node.h"
#include "parser.h"

#include <memory>


// FIXME: turn into struct?
Token::Token(char character, const TokenType type, int level)
    : character {character}, type {type}, level {level}
{
};

NewickString::NewickString(std::vector<char> characters)
    : tokens { std::vector<Token>()}
{
    int level { 0 };
    min_level = 0;
    for (char character : characters) {
        switch (character) {
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

std::unique_ptr<Node> NewickString::to_node() const {
    auto root_tokens { std::vector<Token>() };

    // Parse root name and length. We traverse the tokens starting at the end, and collect everything
    // except closing braces until we hit a higher level.
    for (unsigned long i = tokens.size(); i > 0; i--) {
        Token token = tokens[i - 1];
        if (token.character == '\0') {  // Ignore zero-terminator.
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
    std::ranges::reverse(root_tokens);

    std::string name;
    std::string length;
    bool in_name { true };
    for (auto token : root_tokens) {
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

    // Instantiate a Node ...
    auto node {std::make_unique<Node>(name, length)};
    // ... and add children recursively.
    for (std::vector descendants { get_descendants() }; auto & descendant : descendants) {
        node->add_child(descendant.to_node());
    }
    return node;
};


/*
 * Extracts immediate children as NewickStrings.
 */
std::vector<NewickString> NewickString::get_descendants() const {
    auto descendants { std::vector<NewickString>() };
    auto descendant_tokens { std::vector<Token>() };
    bool comma { false };

    for (auto token : tokens) {
        if (token.type == COMMA && token.level == min_level + 1) {
            // A comma separating immediate children.
            comma = true;
            descendants.emplace_back(descendant_tokens);
            descendant_tokens = std::vector<Token>();
        } else if (token.level > min_level) {  // We are on the children level.
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


std::unique_ptr<Node> parse(std::vector<char> characters) {
    return NewickString(std::move(characters)).to_node();
}
