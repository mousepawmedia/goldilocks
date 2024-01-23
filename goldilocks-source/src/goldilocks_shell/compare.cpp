#include "../include/goldilocks/goldilocks_shell.hpp"

int Goldilocks_Shell::compare(Alias::commands& options)
{
    if(Goldilocks_Shell::check_tests(options)){
        std::cout<<"Compare running\n";
    }
    else{
        std::cout<<"Compare failed\n";
    }

    return 0;
}
