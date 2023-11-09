#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <vector>
#include <memory>
#include "runnable.hpp"

/* The NodeBase class was an idea from ChatGPT. It
 * was created to allow a container of both Suites
 * and Tests, as opposed to Node<Derived>. */
class BaseNode {
public:
    virtual ~BaseNode() = default;

    std::string name;
    std::vector<std::shared_ptr<BaseNode>> children;

    void add_child(std::shared_ptr<BaseNode> child) {
        children.push_back(child);
    }

    bool has_child(const std::string& child_name) const {
        for (const auto& child : children) {
            if (child->name == child_name) {
                return true;
            }
        }
        return false;
    }

    std::shared_ptr<BaseNode> get_child(const std::string& child_name) const {
        for (const auto& child : children) {
            if (child->name == child_name) {
                return child;
            }
        }
        return nullptr;
    }

    void print_tree(std::string prefix = "") const {
        std::cout << prefix << "- " << name << std::endl;
        for (const auto& child : children) {
            child->print_tree(prefix + "  ");
        }
    }
};
//class NodeBase {
//public:
//    virtual ~NodeBase() = default;

//};

/* This is a CTRP (Curiously recurring template pattern)
 * class. Allowing it to have access to both Tests and
 * Suites information even though it is just a Node class. */

template<class Derived>
class Node: public BaseNode
{
public:
    using shr_nodeptr = std::shared_ptr<Derived>;

    Node(std::string_view sent_name);
    virtual ~Node() = default;

    std::string_view            name;
    std::vector<BaseNode>    children;
//    std::shared_ptr<Derived>    suite;
//    std::function<void(void)>   func{[] {return "Empty function\n";}};
};

#endif // NODE_HPP

