#include "../include/goldilocks/coordinator.hpp"

/* Looks for node with node_path value. Returns a shared_ptr if found. */
Coordinator::node_ptr Coordinator::find_node(commands node_path, node_ptr node)
{
    
    // NULL Safety check.
    if(node == NULL){
        channel<<"\n node is null. Something went wrong."<< IOCtrl::endl;
        return NULL;
    }
    
    /* If there is node_path is empty, return the
     * current node. */
    if(node_path.empty()){
        return node;
    }
    // Check if node_name matches the first string in the node_path container.
    auto search_node{std::find_if(node->children.begin(), node->children.end(), [&node_path] (auto& searched_node){
         return searched_node->node_name == node_path.front();
    })};

    /* If the search_node matches, delete the first
     * element in the container, and recursively call
     * find_node with new string for which to look. */
    if(search_node != node->children.end()){
        node_path.erase(node_path.begin());
        return Coordinator::find_node(node_path, std::make_shared<Node>(search_node->get()->node_name));
    }
    /* If the node_name does not match any nodes,
     * print message and return nullptr. */
    else{
        channel <<"\nCannot find: '"<<node_path.front() <<"' on the node: "<<node->node_name<< IOCtrl::endl;
        // return std::shared_ptr<Node>(nullptr);
        return nullptr;
    }
}
// Here is a non-recursive version if wanted to be used instead.
/*    while(!node_path.empty()){
        auto search_node{std::find_if(node->children.begin(), node->children.end(), [&node_path] (auto& searched_node){
        return searched_node->node_name == node_path.front();
        })};
        if(search_node != node->children.end()){
            node = *search_node;
            node_path.erase(node_path.begin());
        }
        else{
            return nullptr;
        }
    }
    return node;
}
//*/
