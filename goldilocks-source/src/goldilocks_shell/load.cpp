#include "../include/goldilocks/goldilocks_shell.hpp"

/* Takes the container of arguments/options, and sends it
 * to process_tests to be tokenized, then calls load_node
 * to add the tests to be added. */
int Goldilocks_Shell::load(commands& options){

    auto processed_commands{coordinate.process_tests(options)};
    coordinate.add_node(processed_commands.front(), processed_commands, coordinate.root_node);
  
//     for(auto& test: processed_commands){
//         auto searched_map {coordinate.available_suites.find(test)};
//         if(searched_map != coordinate.available_suites.end()){
//             std::cout<<"\nFound suite in map: "<< searched_map->first <<'\n';
//             coordinate.load_node(test, processed_commands, root_node);
//         }
//         else std::cout<<"Did not find test: "<< test<<'\n';
//     }

//     for(auto& test: processed_commands){
//         auto searched_test {coordinate.find_node(options, root_node)};
//         if(searched_test != nullptr){
//             std::cout<< "Found test: "<< searched_test->name <<'\n';
//         }
//     }
    /* TODO Run is_suite here to check if the call was
    * for a suite or test? */
//     std::variant<Suites, Tests> getType{Coordinator::is_suite(processed_commands.end()};
//     if(std::holds_alternative<Suites>(getType)){
//         std::cout<<"Suites class was called\n";
//     }
//     else{
//         std::cout<<"Tests class was called\n";
//     }

//     Coordinator::load_node(processed_commands.front(), processed_commands, Goldilocks_Shell::root_node);
//     coordinate.load_node(processed_commands.front(), processed_commands, Goldilocks_Shell::root_node);
    
    return 0;
}
