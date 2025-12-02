#include <iostream>
#include <stack>
#include <utility>

#include "node.h"

#include <functional>


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
    } else {
        return 0.0;
    }
}

void Node::add_child(Node *child) {
    (this->children).push_back(child);
}

void Node::visit(int level) const {
    for (int i = 0; i < level; i++) {
        if (i + 1 < level) {
            std::cout << "  ";
        } else {
            std::cout << "|-";
        }
    }
    std::cout << name << " : " << branch_length << " children: " << children.size() << std::endl;
    for (auto &i: children) {
        i->visit(level + 1);
    }
}


class Pair {
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

        // Repeatedly we will then pop all the elements from the stack till popped
        // element is last children of top of the stack
        while (!stack.empty() && temp->childrenIndex ==
                static_cast<int>(stack.top()->node->children.size()) - 1)
        {
            temp = stack.top();
            stack.pop();
            postorderTraversal.push_back(temp->node);
        }

        // If stack is not empty, then simply assign current to the next children
        // of top of stack.
        if (!stack.empty())
        {
            current = stack.top()->node->children[static_cast<unsigned>(temp->childrenIndex) + 1];
            currentIndex = temp->childrenIndex + 1;
        }
    };
    return postorderTraversal;
}


Node* Node::resolve_polytomies() {
    Node* node {this};
    if (this->children.size() > 2) {
        node = new Node("", "");
        while (this->children.size() > 1) {
            node->add_child(this->children[this->children.size() - 1]);
            this->children.pop_back();
        }
        std::reverse(node->children.begin(), node->children.end());
        this->add_child(node);
    }
    for (auto &i: this->children) {
        i->resolve_polytomies();
    }
    return node;
}

/*
 * FIXME: implement remove_redundant_nodes
*         for n in self.walk(mode='postorder'):
            while n.ancestor and len(n.ancestor.descendants) == 1:
                grandfather = n.ancestor.ancestor
                father = n.ancestor
                if preserve_lengths:
                    n.length += father.length
                if keep_leaf_name:
                    father.name = n.name

                if grandfather:
                    for i, child in enumerate(grandfather.descendants):
                        if child is father:
                            del grandfather.descendants[i]
                    grandfather.add_descendant(n)
                    father.ancestor = None
                else:
                    self.descendants = n.descendants
                    if preserve_lengths:
                        self.length = n.length

 */
void Node::remove_redundant_nodes() {
    for (auto &n: this->postorder_traversal()) {
        if (n->children.size() == 1) {
            Node* child = n->children[0];
            std::cout << n->name << std::endl;
            // Replace the only child with the grandchildren (if any).
            // Adapt branch_lengths. Adapt name.
            // delete the only child.
            n->name = child->name;
            //n->branch_length : FIXME: must convert to float, add, convert back to string
            n->children.pop_back();
            for (auto &i: child->children) {
                n->add_child(i);
            }
            delete child;
        };
    }
}
