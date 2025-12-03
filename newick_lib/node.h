#ifndef NEWICKCPP_NODE_H
#define NEWICKCPP_NODE_H
#include <string>
#include <vector>


class Node {
    std::vector<Node*> children { std::vector<Node*>() };

public:
    std::string name;
    std::string branch_length;
    explicit Node(std::string name, std::string branch_length);
    ~Node();                            // destructor
    Node(const Node&);             // copy constructor
    [[nodiscard]] double branch_length_as_float() const;
    void add_child(Node* child);
    void visit(int level = 0) const;
    std::vector<Node*> postorder_traversal();
    Node* resolve_polytomies();
    void remove_redundant_nodes();
    std::string to_newick(int level=0) const;
    std::string ascii_art() const;
};


#endif //NEWICKCPP_NODE_H