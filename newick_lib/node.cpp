#include <iostream>

#include "node.h"


Node::Node(std::string name, std::string branch_length, Node* parent)
    : name {name}, branch_length {branch_length}, parent {parent}
{
    std::cout << "Node(" << name << " : " << branch_length << ")" << std::endl;

}

Node::~Node()
= default;

Node::Node(const Node& N)
    : name {N.name}, branch_length {N.branch_length}, parent {N.parent}
{
}

double Node::branch_length_as_float() {
    if (!branch_length.empty()) {
        return std::stod(branch_length);
    } else {
        return 0.0;
    }
}

void Node::add_child(Node child) {
    child.parent = this;
    children.push_back(child);
}
