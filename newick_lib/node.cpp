#include <regex>
#include <set>
#include <stack>
#include <utility>

#include "node.h"


Node::Node(std::string name, std::string branch_length)
    : name{std::move(name)}, branch_length{std::move(branch_length)} {
    children = std::vector<Node *>();
}

Node::~Node()
= default;

Node::Node(const Node &N)
    : name{N.name}, branch_length{N.branch_length} {
}

double Node::branch_length_as_float() const {
    if (!branch_length.empty()) {
        return std::stod(branch_length);
    }
    return 0.0;
}

void Node::add_child(Node *child) {
    (this->children).push_back(child);
}

/*
 * Visit each node in a tree, possibly mutating it.
 */
void Node::visit(const std::function<void(Node*)>& visitor, int level) {
    visitor(this);
    for (auto &i: children) {
        i->visit(visitor, level + 1);
    }
}


class Pair {  // Helper class for postorder traversal algorithm.
public:
    Node *node;
    int childrenIndex;

    Pair(Node *_node, int _childrenIndex)
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

    Node *current { this };

    while (!stack.empty() || current != nullptr) {
        if (current != nullptr) {
            // Traverse down to the leftmost leaf.
            // Push the current node and its index on the stack
            stack.push(new Pair(current, currentIndex));
            currentIndex = 0;

            if (!current->children.empty()) {
                // Traverse to the leftmost child
                current = current->children[0];
            } else {
                // No children, mark current as null and look what's left on the stack.
                current = nullptr;
            }
            continue;
        }
        // Top node on the stack is returned
        Pair* temp = stack.top();
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
            current = stack.top()->node->children[static_cast<unsigned>(temp->childrenIndex) + 1];
            currentIndex = temp->childrenIndex + 1;
        }
        delete temp;
    };
    return postorderTraversal;
}


Node* Node::resolve_polytomies() {
    Node* node {this};
    if (this->children.size() > 2) {  // A polytomy.
        // We insert a new node as parent for all but one child.
        node = new Node("", "");
        while (this->children.size() > 1) {
            node->add_child(this->children[this->children.size() - 1]);
            this->children.pop_back();
        }
        // We added children from the back, so we reverse to keep the order.
        std::reverse(node->children.begin(), node->children.end());
        this->add_child(node);
    }
    for (auto &i: this->children) {  // Recurse.
        i->resolve_polytomies();
    }
    return node;
}

/*
 * Remove redundant nodes.
 */
Node* Node::remove_redundant_nodes() {
    for (auto &n: this->postorder_traversal()) {
        if (n->children.size() == 1) {
            Node* child = n->children[0];
            // Replace the only child with the grandchildren (if any).
            // Adapt branch_length. Adapt name. Delete the only child.
            n->name = child->name;
            if (!n->branch_length.empty()) {
                double length {n->branch_length_as_float()};
                length += child->branch_length_as_float();
                n->branch_length = std::to_string(length);
            }
            n->children.pop_back();
            for (auto &i: child->children) {
                n->add_child(i);
            }
            delete child;
        };
    }
    return this;
}

/*
 * Format the tree as Newick string.
 */
std::string Node::to_newick(int level) const {
    std::string newick { std::string("") };
    if (!this->children.empty()) {
        newick.append("(");
        for (auto &n: this->children) {
            if (newick.back() != '(') {
                newick.append(",");
            }
            newick.append(n->to_newick(level + 1));
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


std::vector<std::string> Node::ascii_art(const std::string &char1, unsigned long maxlen) {
    if (maxlen == 0) {  // Determine the maximal length of a node label.
        std::vector<Node*> nodes {this->postorder_traversal()};
        auto max_node = std::max_element(
            nodes.begin(),
            nodes.end(),
            [](const auto& s1, const auto& s2){return s1->name.size() < s2->name.size();}
            );
        if (max_node != nodes.end()) {
            maxlen = (*max_node)->name.size();
        }
    }
    auto pad {std::string(maxlen + 2, ' ')};
    auto pad_minus_1 {std::string(maxlen + 1, ' ')};
    std::vector<std::string> lines {std::vector<std::string>()};

    if (!this->children.empty()) {
        auto result {std::vector<std::string>()};  // accumulated lines of all child nodes.

        for (unsigned int i = 0; i < this->children.size(); i++) {
            std::string char2 {"\u2500"};
            if (this->children.size() == 1) {  // only one child
                char2 = "\u2500";
            } else if (i == 0) {  // first child of several
                char2 = "\u250c";
            } else if (i == this->children.size() - 1) {  // last child of several
                char2 = "\u2514";
            }
            // Recursively compute the lines of the ascii representation of child nodes.
            std::vector<std::string> res { this->children[i]->ascii_art(char2, maxlen)};
            result.reserve(result.size() + res.size());
            result.insert(result.end(),res.begin(),res.end());
            result.emplace_back("\u2502");  // add a line starting with a pipe to space nodes vertically.
        }
        result.pop_back();  // remove the added line after the last child.
        int end {static_cast<int>(result.size())};
        int lo {0};  // The index of the line of the first immediate child
        int hi {0};  // The index of the line of the last immediate child
        int mid {0}; // The index of the line where to attach the parent node

        // find lo and hi as index of the line that starts with "\u250c" and "\u2514" respectively.
        for (int i {0}; i < end; i++) {
            if (result[static_cast<unsigned long>(i)].rfind("\u250c", 0) == 0) {
                lo = i;
            } else if (result[static_cast<unsigned long>(i)].rfind("\u2514", 0) == 0) {
                hi = i;
                break;  // If we found the last child, we're done here.
            } else if (result[static_cast<unsigned long>(i)].rfind("\u2500", 0) == 0) {
                // If there's only one child, we determine it by the leading "\u2500"
                mid = i;
            }
        }
        if (hi != 0) {  // More than one child, attach the parent centered.
            mid = lo + ((hi - lo) / 2);
        }
        // loop over result by index; compute prefix; prepend prefix.
        // prefix is expected to be maxlen + 2
        for (int i {0}; i < end; i++) {
            std::string prefix { pad };
            std::string line {result[static_cast<unsigned long>(i)]};

            if (i == mid) {
                prefix = char1 + this->name + dashes(maxlen - this->name.size()) + "\u2500";
                if (line.rfind(pad + "\u2502", 0) == 0) {
                    // The tree has more than one nesting level.
                    prefix += "\u2502";
                    line.erase(0, 1);
                } else if (line.rfind("\u2500", 0) == 0 && hi != 0) {
                    // a crossing - parent with odd number (>= 3) of chlidren.
                    line.erase(0, 3);
                    line.insert(0, "\u253c");
                } else if (line.rfind("\u2502", 0) == 0) {
                    // Replace pipe with T-crossing character.
                    line.erase(0, 3);
                    line.insert(0, "\u2524");
                }
            } else if ((char1 == "\u2514") && i < mid) {
                // Before the last child
                prefix = "\u2502" + pad_minus_1;
            } else if ((char1 == "\u250c") && i > mid) {
                // After the first child
                prefix = "\u2502" + pad_minus_1;
            } else if (i > lo && i < hi && line.rfind("\u2500", 0) == 0) {
                // An intermediate child node.
                prefix = pad + "\u251c";
                line.erase(0, 3);
            } else if (i > lo && i < hi && line.rfind("\u2502", 0) != 0) {
                // Add a pipe if there isn't already one.
                prefix = pad + "\u2502";
                line.erase(0, 1);
            }
            lines.emplace_back(prefix + line);
        }
    } else {
        lines.emplace_back(char1 + this->name);
    }
    // We remove lines containing only spaces and pipes.
    auto indices {std::vector<unsigned long>()};

    for (unsigned long i {0}; i < lines.size(); i++) {
        // Hacky way to work around the missing (simple) support for unicode in regexes.
        std::string non_space;
        for (char j : lines[i]) {
            if (j != ' ') {
                non_space += j;
            }
        }
        if (non_space == "\u2502" || non_space == "\u2502\u2502") {
            // More than two pipes should not have accumulated at this point, because we compactify at
            // each recursion level.
            indices.emplace_back(i);
        }
    }
    while (!indices.empty()) {  // We consume indices from back to start and erase lines accordingly.
        lines.erase(std::next(lines.begin(), static_cast<int>(indices.back())));
        indices.pop_back();
    }
    return lines;
};
