#include "goldilocks/coordinator/coordinator.hpp"

Node* Coordinator::find_node(Alias::commands name, Node* node){
	auto searched_node = std::find_if(node->children.begin(), node->children.end(), [&name](auto& node_){return name == node_->name;});
	return (searched_node != std::end(node->children))? searched_node->get(): nullptr;
}

Node* Coordinator::find_node(std::vector<std::string> name, Node* node){
	auto searched_node = std::find_if(node->children.begin(), node->children.end(), [&name](auto& node_){return name == node_->name;});
	return (searched_node != std::end(node->children))? searched_node->get(): nullptr;
}
