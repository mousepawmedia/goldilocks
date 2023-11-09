#include "../include/goldilocks/goldilocks_shell.hpp"

int Goldilocks_Shell::about(commands& options){
    
    if(Goldilocks_Shell::check_tests(options)){
        std::cout<<"About running\n";
    }
    else{
        channel <<"Cannot find the test "<< options.front() <<" on available tests\n" << IOCtrl::endl;
    }
    return 0;
}
