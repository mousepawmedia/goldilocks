#include "../include/goldilocks/goldilocks_shell.hpp"

int Goldilocks_Shell::kamikaze(Alias::commands& options)
{
    if(Goldilocks_Shell::check_tests(options)){
        std::cout<<"Kamikaze running\n";
    }
    else{
        std::cout<<"Kamikaze failed\n";
    }
    return 0;
}
