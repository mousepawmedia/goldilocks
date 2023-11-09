#include "../include/goldilocks/goldilocks_shell.hpp"

/* Goldilocks_Shell constructor that passes the 
 * name provided by the user for the shell name. 
 * It will be called Blueshell by default. */
Goldilocks_Shell::Goldilocks_Shell(const std::string& shell_name)
: Blueshell(shell_name)
{
    /* Add names of functions that need to bypass the
     * process_command check for number of args/options.*/
     Blueshell::add_skipped_command("list");
     Blueshell::add_skipped_command("find");
     
    Goldilocks_Shell::register_defaults();
    Blueshell::initial_shell();
}
