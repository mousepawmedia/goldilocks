#include "../include/goldilocks/goldilocks_shell.hpp"

int Goldilocks_Shell::benchmark(Alias::commands& options)
{
    if(Goldilocks_Shell::check_tests(options)){
        std::cout<<"Benchmark running\n";
    }
    else{
        std::cout<<"Benchmark failed\n";
    }
    

    return 0;
}
