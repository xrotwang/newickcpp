#ifndef NEWICKCPP_NODE_H
#define NEWICKCPP_NODE_H
#include <functional>
#include <memory>
#include <string>
#include <vector>


class Node {
    std::vector<std::unique_ptr<Node>> children;

public:
    std::string name;
    std::string branch_length;
    explicit Node(std::string name, std::string branch_length);
    ~Node() = default;                            // destructor
    // Recommended: Prevent copying of the class instance
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    // Allow moving the class instance
    Node(Node&&) = default;
    Node& operator=(Node&&) = default;

    void add_child(std::unique_ptr<Node> node) {
        // Use std::move to transfer ownership to the vector
        children.emplace_back(std::move(node));
    }

    // Access elements (e.g., using a raw pointer or reference to const)
    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& get_children() const {
        return children;
    }

    [[nodiscard]] double branch_length_as_float() const;
    void visit(const std::function<void(Node*)>& visitor, int level = 0);
    std::vector<Node*> postorder_traversal();
    Node* resolve_polytomies();
    Node* remove_redundant_nodes();
    [[nodiscard]] std::string to_newick(int level=0) const;
    std::vector<std::string> ascii_art(const std::string &char1="\u2500", unsigned long max_len=0);
};


#endif //NEWICKCPP_NODE_H