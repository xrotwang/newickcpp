#ifndef NEWICKCPP_NODE_H
#define NEWICKCPP_NODE_H
#include <functional>
#include <string>
#include <vector>


class Node {

public:
    std::string name;
    std::string branch_length;
    std::vector<Node*> children { std::vector<Node*>() };
    explicit Node(std::string name, std::string branch_length);
    ~Node();                            // destructor
    Node(const Node&);             // copy constructor
    [[nodiscard]] double branch_length_as_float() const;
    void add_child(Node* child);
    void visit(const std::function<void(Node*)>& visitor, int level = 0);
    std::vector<Node*> postorder_traversal();
    Node* resolve_polytomies();
    Node* remove_redundant_nodes();
    [[nodiscard]] std::string to_newick(int level=0) const;
    std::vector<std::string> ascii_art(const std::string &char1="\u2500", unsigned long maxlen=0);
};


#endif //NEWICKCPP_NODE_H