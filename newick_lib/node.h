//
// Created by robert on 11/29/25.
//

#ifndef NEWICKCPP_NODE_H
#define NEWICKCPP_NODE_H
#include <string>


class Node {
    public:
    std::string name;
    float branch_length;
    Node* parent {nullptr};
    explicit Node(std::string name, float branch_length, Node* parent);
    ~Node();                            // destructor
    Node(const Node&);             // copy constructor
    Node& operator=(const Node &); // assignment
};


#endif //NEWICKCPP_NODE_H