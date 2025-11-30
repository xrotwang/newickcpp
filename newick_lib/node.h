//
// Created by robert on 11/29/25.
//

#ifndef NEWICKCPP_NODE_H
#define NEWICKCPP_NODE_H
#include <string>
#include <vector>


class Node {
    std::vector<Node> children { std::vector<Node>() };

public:
    std::string name;
    std::string branch_length;
    Node* parent {nullptr};
    explicit Node(std::string name, std::string branch_length, Node* parent);
    ~Node();                            // destructor
    Node(const Node&);             // copy constructor
    Node& operator=(const Node &); // assignment
    double branch_length_as_float();
    void add_child(Node child);
};


#endif //NEWICKCPP_NODE_H