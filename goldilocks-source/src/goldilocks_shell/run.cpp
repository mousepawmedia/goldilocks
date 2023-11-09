#include "../include/goldilocks/goldilocks_shell.hpp"
#include <numeric>
#include <string>

int Goldilocks_Shell::run(commands& options)
{
    if(Goldilocks_Shell::check_tests(options)){
        
    }
    else{
        std::cout<<"Run failed\n";
    }
    return 0;
}
