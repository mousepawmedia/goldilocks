#include "../include/goldilocks/goldilocks_shell.hpp"
#include "iosqueak/stringy.hpp"

bool Goldilocks_Shell::check_tests(commands& options, const std::string& test_name)
{
    /* Get iterator to matching command. Used to check number
     * of arguments and options. */    
    auto find_command{std::find_if(stored_commands.get_set().begin(), stored_commands.get_set().end(), [&test_name](auto& cmd) {
			return cmd.func_name == test_name;
		})};
    
    /* If test is not found, print message and return. */
    if(find_command == stored_commands.get_set().end()){
        channel <<"Command "<< test_name << " not found\n";
        return false;
    }

    // if(find_command->number_of_args || (!find_command->number_of_args && options.size())){
    if(find_command->number_of_args || options.size()){
        channel << IOCtrl::n;

        for(size_t i{0}; i < ((find_command->number_of_args)? find_command->number_of_args: find_command->number_of_options); ++i){
            /*Check if first option is a valid test, and no
            * option for number of tests sent. Not optimal
            * as you need to create a vector and a deque.*/
            std::deque<std::string> test_name{options[i]};
            
            auto split_test_name{coordinate.process_tests(test_name)};
                
            // Search for node containing test name.
            auto find_test{coordinate.find_node(split_test_name, coordinate.root_node)};
            
            /* If node not found, clear the container and return. */
            if(find_test == nullptr){          
                options.clear();
                return false;
            }
        }
    }

    
    /* If number of tests is supplied, check if it
     * is an integer, otherwise print message. */
    int number {0};

    if(find_command->number_of_args && find_command->number_of_args){
        std::cout<<"Inside options number check\n";
        //Check if sent option is a number.
        try{
            number = {stoi(options.back())};
        }
        /* If option is not a number print message. */
        catch(const std::invalid_argument){
            channel<< options.back() << " is not a number. Please enter number of tests to run.\n" << IOCtrl::endl;
            return false;
        }
    }

    channel << "Running test "<< test_name << ' ' << ((number)? number: find_command->number_of_options) << " times\n" << IOCtrl::endl;
    options.clear();
   return true;
    
}
