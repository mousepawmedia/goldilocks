#ifndef NODE_HPP
#define NODE_HPP

#include "goldilocks/coordinator/base_node.hpp"
#include "goldilocks/coordinator/alias.hpp"
#include <iostream>
#include <memory>
#include <string_view>

class Node
{
public:

    Node(std::string_view sent_name);
    virtual ~Node() = default;

	std::string_view	node_name;
	Alias::node_vector	children;

	bool has_child(std::string_view);
	Alias::uniq_node get_child(std::string_view);
};

#endif // NODE_HPP
