#include <algorithm>
#include <iterator>
#include <regex>
#include <set>
#include <stack>
#include <utility>

#include "node.h"

#include <iostream>


Node::Node(std::string name, std::string branch_length)
    : name{std::move(name)}, branch_length{std::move(branch_length)} {
}


double Node::branch_length_as_float() const {
    if (!branch_length.empty()) {
        return std::stod(branch_length);
    }
    return 0.0;
}

/*
 * Visit each node in a tree, possibly mutating it.
 */
void Node::visit(const std::function<void(Node*)>& visitor, const int level) {
    visitor(this);
    for (auto &i: children) {
        i->visit(visitor, level + 1);
    }
}


class Pair {  // Helper class for postorder traversal algorithm.
public:
    Node *node;
    int childrenIndex;

    Pair(Node *_node, const int _childrenIndex)
        : node {nullptr}, childrenIndex { 0 }
    {
        node = _node;
        childrenIndex = _childrenIndex;
    }
};


std::vector<Node*> Node::postorder_traversal() {
    int currentIndex = 0;
    std::stack<Pair*> stack;
    std::vector<Node*> postorderTraversal ;

    auto current { this };

    while (!stack.empty() || current != nullptr) {
        if (current != nullptr) {
            // Traverse down to the leftmost leaf.
            // Push the current node and its index on the stack
            stack.push(new Pair(current, currentIndex));
            currentIndex = 0;

            if (!current->children.empty()) {
                // Traverse to the leftmost child
                current = &*current->get_children()[0];
            } else {
                // No children, mark current as null and look what's left on the stack.
                current = nullptr;
            }
            continue;
        }
        // Top node on the stack is returned
        const Pair* temp = stack.top();
        stack.pop();
        postorderTraversal.push_back(temp->node);

        // Now add siblings of the last leaf.
        while (!stack.empty() && temp->childrenIndex ==
                static_cast<int>(stack.top()->node->children.size()) - 1)
        {
            delete temp;
            temp = stack.top();
            stack.pop();
            postorderTraversal.push_back(temp->node);
        }

        // If stack is not empty, go one level up, and continue.
        if (!stack.empty())
        {
            current = &*(stack.top()->node)->get_children()[static_cast<unsigned>(temp->childrenIndex) + 1];
            currentIndex = temp->childrenIndex + 1;
        }
        delete temp;
    };
    return postorderTraversal;
}


Node* Node::resolve_polytomies() {
    if (this->children.size() > 2) {  // A polytomy.
        // We insert a new node as parent for all but one child.
        this->children.emplace_back(std::make_unique<Node>("", ""));
        // Move all children but the first and the newly created child to the new node.
        std::move(
            this->children.begin() + 1,
            this->children.end() - 1,
            std::back_inserter(this->children.back()->children));
        this->children.erase(this->children.begin() + 1, this->children.end() - 1);
    }

    for (auto &i: this->children) {  // Recurse.
        i->resolve_polytomies();
    }
    return this;
}

/*
 * Remove redundant nodes.
 */
Node* Node::remove_redundant_nodes() {
    for (const auto &n: this->postorder_traversal()) {
        if (n->children.size() == 1) {
            const double length {n->branch_length_as_float() + n->get_children()[0]->branch_length_as_float()};
            if (!n->branch_length.empty()) {
                n->branch_length = std::to_string(length);
            }
            n->name = n->get_children()[0]->name;
            std::ranges::move(
                n->children[0]->children,
                std::back_inserter(n->children));
            n->children.erase(n->children.begin(), n->children.begin() + 1);
        };
    }
    return this;
}

/*
 * Format the tree as Newick string.
 */
std::string Node::to_newick(const int level) const {
    auto newick { std::string("") };
    if (!this->children.empty()) {
        newick.append("(");
        for (auto &n: this->children) {
            if (newick.back() != '(') {
                newick.append(",");
            }
            newick.append(n->to_newick(level + 1));  // recurse.
        }
        newick.append(")");
    }
    newick.append(this->name);
    if (!this->branch_length.empty()) {
        newick.append(":");
        newick.append(this->branch_length);
    }
    if (level == 0) {
        newick.append(";");
    }
    return newick;
}


std::string dashes(unsigned long n) {
    std::string res;
    if (n <= 0) {
        return res;
    }
    for (unsigned long i = 0; i < n; i++) {
        res += "\u2500";
    }
    return res;
}

/*
 * Create a string with pipes at the positions matching downward pipes in the line above.
 *
 * in  = "   │   ┌x"
 * out = "   │   │"
 */
std::string pipes(std::string s) {
    std::string res;
    while (!s.empty()) {
        // look for |, |-, /
        std::size_t next_pipe {s.find("\u2502")};
        if (next_pipe == std::string::npos) {
            next_pipe = s.find("\u250c");
        }
        if (next_pipe == std::string::npos) {
            next_pipe = s.find("\u251c");
        }
        if (next_pipe == std::string::npos) {
            break;
        }
        res += s.substr(0, next_pipe);
        res += "\u2502";
        s.erase(0, next_pipe + std::string("\u2502").size());
    }
    return res;
}


std::vector<std::string> Node::ascii_art(unsigned long max_len) {
    if (max_len == 0) {  // Determine the maximal length of a node label.
        std::vector<Node*> nodes {this->postorder_traversal()};
        auto max_node = std::ranges::max_element(
            nodes,
            [](const auto& s1, const auto& s2){return s1->name.size() < s2->name.size();}
        );
        if (max_node != nodes.end()) {
            max_len = (*max_node)->name.size();
        }
    }
    auto pad {std::string(max_len + 1, ' ')};
    auto lines {std::vector<std::string>()};

    if (this->children.empty()) {
        lines.push_back(this->name);
        return lines;
    }

    int mid_child {static_cast<int>(this->children.size()) / 2};
    bool even_number_of_children {static_cast<int>(this->children.size()) % 2 == 0};
    int child_index {0};
    bool in_children {false};

    for (const auto & n : this->children) {
        // Recursively compute the ascii representation of the child node.
        std::vector child_lines { n->ascii_art(max_len)};
        unsigned long mid {child_lines.size() / 2};
        const bool last_child {n == this->children.back()};

        // Loop over child_lines, and pad them or attach the parent name.
        for (unsigned long i = 0; i < child_lines.size(); i++) {
            std::string full_line {pad};
            if (child_index == mid_child && i == mid) {
                // Attach the parent name.
                if (even_number_of_children && last_child) {
                    /*
                     * To avoid representations like
                     *   /-a
                     * c-\-b
                     * we insert an additional line if there's an even number of children.
                     */
                    mid --;  // Decrement the indicator for the middle line.
                    if (i > 0) {
                        lines.push_back(this->name + dashes(max_len + 1 - this->name.size()) + "\u2524" + pipes(child_lines[i - 1]));
                    } else {
                        lines.push_back(this->name + dashes(max_len + 1 - this->name.size()) + "\u2524");
                    }
                } else {
                    full_line = this->name + dashes(max_len - this->name.size()) + "\u2500";
                }
            }

            const std::string& line {child_lines[i]};
            if (line.find(' ', 0) == 0) { // line starts with space:
                if (in_children) {
                    // - either prepend pipe (if in between first child and last child)
                    full_line.append("\u2502");
                } else {
                    // - or prepend space otherwise
                    full_line.append(" ");
                };
            } else { // line does not start with space:
                if (child_index == 0 && last_child) {
                    // - prepend "-" if only one child
                    full_line.append("\u2500");
                } else if (!in_children) {
                    // - prepend "/" if first child
                    full_line.append("\u250c");
                    in_children = true;
                } else if (last_child) {
                    // - prepend "\" if last child
                    full_line.append("\u2514");
                    in_children = false;
                } else {
                    // - prepend "|-" or "-|-" if in between children
                    if (child_index == mid_child && i == mid) {
                        full_line.append("\u253c");
                    } else {
                        full_line.append("\u251c");
                    }
                }
            }
            lines.push_back(full_line + line);
        }
        child_index++;
    }
    return lines;
};
