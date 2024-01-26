#ifndef NODE_HPP
#define NODE_HPP

#include "goldilocks/coordinator/alias.hpp"

#include <iostream>
#include <memory>
#include <string_view>

class Node
{
public:

    Node(std::string_view sent_name);
    virtual ~Node() = default;

	std::string			node_name;
	Alias::node_vector	children;

	/* Checks if a node exists in the children vector. */
	bool has_child(std::string_view);

	/* Checks if a node is in the children vector, and
	 * returns (moves) the unique_ptr to the caller. */
	Alias::smrt_node get_child(std::string_view);
};

#endif // NODE_HPP
