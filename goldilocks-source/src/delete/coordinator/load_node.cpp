#include "../include/goldilocks/coordinator.hpp"

/* Adds a new child to the current node. It calls find_node to check
 * if nodes children container is empty. If so, root is the current node, otherwise calls find_node. */
void Coordinator::load_node(std::string& node_name, commands& on_node, node_ptr node){

    /* Check if the test the user wants to load is already loaded,
     * if so, print message. */
    if(on_node.size() == 1 && Coordinator::find_child(node_name, node) != nullptr)
    {
        std::cout<<"\nNode "<<node_name<<" is already loaded\n";
        return;
    }

    /* If on_node is empty, add child to node. on_node is a container
     * that is used to hold the searched string. */
    if(on_node.empty())
    {
        channel << "\nLoad all available suites (yes/No)? " << IOCtrl::end;
        std::string answer;
        std::getline(std::cin, answer);
        if(answer == "y" || answer == "yes"){
            //TODO loop through all Suites/tests and load into tree.
            channel << "\nLoading all suites.\n"
                    << IOCtrl::endl;
        }
        return;
    }
    else if(!on_node.empty())
    {
        auto check_node{Coordinator::find_child(node_name, node)};

        if(check_node == nullptr)
        {
            //TODO Find suites/tests for each call and load them into tree.
            //FIX!! Pass Node to make_shared<Node>, not string!!!

            node->children.push_back(std::make_shared<Node>(node_name));

            on_node.erase(on_node.begin());

            node_name = on_node.front();
            node = node->children.back();
            if(on_node.empty()){
                return;
            }

            return Coordinator::load_node(node_name, on_node, node);
        }
        else
        {
            on_node.erase(on_node.begin());
 
            node_name = on_node.front();
            node = check_node;
            if(on_node.empty()){
                return;
            }

            Coordinator::load_node(node_name, on_node, check_node);
        }
    }
}

    //!!!!!! Make a deque(commands) and send to load_node if first option is a test/suite?????
    
    // Variables to print area where test was not found.
//     bool found{false};
//     std::string last_word;
//     
//     // Variable to build string to pass to load_node.
//     std::string path;
//     /* Check if processed_commands has Suite or Test
//      * to load. Print message if not. */
//     for(size_t i{0}; i < ((coordinate.available_suites.size() < coordinate.available_tests.size())? coordinate.available_tests.size(): coordinate.available_suites.size()); ++i)
//     {
//         if(i < coordinate.available_suites.size())
//         {
//             auto searched_map {coordinate.available_suites.find(processed_commands[i])};
//             if(searched_map != coordinate.available_suites.end()){
//                 std::cout<<"\nFound suite in suites map: "<< searched_map->first <<'\n';
//                 coordinate.load_node(processed_commands[i], processed_commands, root_node);
//                 found = true;
//                 path += processed_commands[i] + ".";
// //                 break;
//             }
//         }
//         if( i < coordinate.available_tests.size())
//         {
//             auto searched_map {coordinate.available_tests.find(processed_commands[i])};
//             if(searched_map != coordinate.available_tests.end()){
//                 std::cout<<"\nFound test in tests map: "<< searched_map->first <<'\n';
//                 coordinate.load_node(processed_commands[i], processed_commands, root_node);
//                 found = true;
//                 path += processed_commands[i] + ".";
// //                 break;
//             }
//         }
//         // Add last found word to be printed if Suite/Test not found.
//         if(processed_commands[i].size() > 0) last_word = processed_commands[i];
//     }
//     if(!found){
//         std::cout<<"\nDid not find test: "<< last_word <<'\n';
//         return 0;
//     }
//     std::cout<<"Path: "<<path<<'\n';
  
