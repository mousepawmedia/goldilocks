#include "../include/goldilocks/goldilocks_shell.hpp"

// Forward declaration of find_children.
// std::string find_children(const std::shared_ptr<Node> node, const std::string& node_path);
std::string find_children(const Node* node, const std::string& node_path);

int Goldilocks_Shell::list_loaded (Alias::commands& options)
{
    channel << IOCtrl::endl;
    
    // If no tests specified, print all loaded tests.
    if(options.empty()){
        for(auto& child: coordinate.root_node->children){
            channel <<child->node_name <<':'<< IOCtrl::endl;
            channel <<find_children(child.get(), std::string())
                    << IOCtrl::endl;
        }
    }
    
    // Print only the tests asked for, if loaded.
    else{
        // Break searched test into tokens.
        auto processed_commands{coordinate.process_tests(options)};
        
        // Check if the test is loaded.
        if(Goldilocks_Shell::check_tests(options)){
            // Find the node and print the children.
            Alias::smrt_node searched_node{coordinate.find_node(processed_commands, coordinate.root_node.get())};
            if(searched_node != nullptr){
                channel<< find_children(searched_node.get(), (searched_node->node_name + "->"))<< IOCtrl::endl;
            }
            
        }
    }
    
    channel<< IOCtrl::endl;
    (void) options; //to suppress unused variable warning
    
     return 0;
}

// std::string find_children(const std::shared_ptr<Node> node, const std::string& node_path){
std::string find_children(const Node* node, const std::string& node_path){
    /* If the children node is empty, print out
     * the node_path and return. */
    if(node->children.size() == 0){
        channel << node_path <<"loaded." << IOCtrl::endl;
        return std::string();
    }
    /* Recursive call to add node_name to the
     * node_path for each child node. */
    for(auto& child: node->children){
        find_children(child.get(), node_path + (child->node_name + "->"));
    }
    return std::string();
}
