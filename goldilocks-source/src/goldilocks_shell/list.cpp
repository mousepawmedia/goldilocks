#include "goldilocks/goldilocks_shell.hpp"

int Goldilocks_Shell::list(Alias::commands& options){
std::cout<<"\nlist called with suites container size: "<< coordinate.root_node->children.size()<<'\n';
//    for(auto& [name, suite]: coordinate.root->children)
//    {
//        Goldilocks_Shell::get_suites(suite.get());
//        // coordinate.  print_suites(suite.get());
//    }
    // for(auto& [name, test]: )
    
    // Suppress unused variable warning.
    (void) options;

    return 0;
}

//void Goldilocks_Shell::get_suites(Suites* suites)
//{
//    std::cout<<"get_suite called with container size: "<< suites->suites_container.size()<<'\n';
//    if(!suites->suites_container.empty()){
//        for(auto& suite: suites->suites_container){
//            std::cout<<"Suite not empty\n";
//            return Goldilocks_Shell::get_suites(suite.get());
//        }
//    }
//    else{
//        for(auto& [key, value]: coordinate.coordinator_send_suites()){
//            std::cout<<"\nSuites: "<<key << " has the following suites:\n";
//            std::cout<<value->name <<":\n";
//            // coordinate.print_container(value);
//            std::cout<<"Tests in " << value.get()->name <<":\n";
//            for(auto& test: value.get()->suites_send_tests()){
//                std::cout<<"Test: "<< test.get()->name << '\n';
//            }
//        }
//        for(auto& [key, value]: coordinate.coordinator_send_tests()){
//            std::cout<<"\nTests: "<<key << " is available\n";
//            // value.get()->printTests(*value);
//        }
//    }

    // Suppress unused variable warning.
    // (void) options;
    // return 0;
//}
// // Forward declaration of find_children.
// std::string find_children(const std::shared_ptr<Node> node, const std::string& node_path);
// 
// int Goldilocks_Shell::list (commands& options)
// {
//     channel << IOCtrl::endl;
//     
//     // If no tests specified, print all loaded tests.
//     if(options.empty()){
//         for(auto& child: root_node->children){
//             channel <<child->node_name <<':'<< IOCtrl::endl;
//             channel <<find_children(child, std::string())
//                     << IOCtrl::endl;
//         }
//     }
//     
//     // Print only the tests asked for, if loaded.
//     else{
//         // Break searched test into tokens.
//         auto processed_commands{Coordinator::process_tests(options)};
//         
//         // Check if the test is loaded.
//         if(Goldilocks_Shell::check_tests(options)){
//             // Find the node and print the children.
//             Coordinator::node_ptr searched_node{Coordinator::find_node(processed_commands, Goldilocks_Shell::root_node)};
//             if(searched_node != nullptr){
//                 channel<< find_children(searched_node, (searched_node->node_name + "->"))<< IOCtrl::endl;
//             }
//             
//         }
//     }
//     
//     channel<< IOCtrl::endl;
//     (void) options; //to suppress unused variable warning
//     
//      return 0;
// }
// 
// std::string find_children(const std::shared_ptr<Node> node, const std::string& node_path){
//     /* If the children node is empty, print out
//      * the node_path and return. */
//     if(node->children.size() == 0){
//         channel << node_path <<"loaded." << IOCtrl::endl;
//         return std::string();
//     }
//     /* Recursive call to add node_name to the
//      * node_path for each child node. */
//     for(auto& child: node->children){
//         find_children(child, node_path + (child->node_name + "->"));
//     }
//     return std::string();
// }
