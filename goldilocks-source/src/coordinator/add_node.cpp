#include "goldilocks/coordinator/coordinator.hpp"

void Coordinator::add_node(Alias::smrt_suite suites, Node* node)
{
	std::cout<<"add_node for suite: "<<suites->name<< " called on node: "<< node->node_name<<"\n";
	node->children.emplace_back(std::move(suites));
}

void Coordinator::add_node(Alias::smrt_test test, Node* node)
{
	std::cout<<"add_node for test: "<<test->name<< " called on node: "<< node->node_name<<"\n";
	node->children.emplace_back(std::move(test));
}
