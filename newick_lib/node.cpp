//
// Created by robert on 11/29/25.
//

#include "node.h"


Node::Node(std::string name, float branch_length, Node* parent)
    : name {name}, branch_length {branch_length}, parent {parent}
{

}

Node::~Node()
= default;

Node::Node(const Node& N)
    : name {N.name}, branch_length {N.branch_length}, parent {N.parent}
{
}