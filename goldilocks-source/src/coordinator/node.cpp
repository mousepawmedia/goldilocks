#include "goldilocks/coordinator/node.hpp"

Node::Node(std::string_view name): node_name{name}{}

bool Node::has_child(std::string_view name){
	for(auto& child: children){
		if(child->node_name == name){
			return true;
		}
	}
	return false;
}

Alias::smrt_node Node::get_child(std::string_view name){
	for(auto& node: children){
		if(node->node_name == name){
			std::cout<<"Node found and moving\n";
			return std::move(node);
		}
	}
	return nullptr;
}
