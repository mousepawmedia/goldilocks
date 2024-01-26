#include "../include/goldilocks/goldilocks_shell.hpp"

/* Takes the container of arguments/options, and sends it
 * to process_tests to be tokenized, then calls find_node
 * to check if the tests are loaded. */
int Goldilocks_Shell::find(Alias::commands& options){
    
    /* Create a string to hold the name of the
     * test to find. Fill the string with the
     * container passes as an argument. Used
     * to display if the test was found. */
    std::string test_name;
    
    std::for_each(options.begin(), options.end(),[&test_name](const auto& word){
        return test_name += word;
    });

    // Send container to be tokenized.
    auto processed_commands{coordinate.process_tests(options)};

	// Check if the test is loaded or not.
	auto searched_node{coordinate.find_node(processed_commands, coordinate.root_node.get())};
    
    // Print message if test was found or not.
    channel << IOCtrl::n << "Test "<< test_name << ((searched_node != nullptr)? " was found.": " was not found.") << IOCtrl::endl;
    
    return 0;
}
