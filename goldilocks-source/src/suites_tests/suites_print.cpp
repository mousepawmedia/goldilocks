#include "goldilocks/suites_tests/suites.hpp"

void Suites::print_container(Suites* suites){

    // if (suites_container.empty() && suite_tests_container.empty()) return;

    if(!suites->suites_container.empty()){

        for(auto& suite: suites->suites_container)
        {
            std::cout <<"Suite "<< suite->name <<" contains:\n";
            std::cout<< suite->name <<" suite continer size is: "<< suite->suites_container.size()<<'\n';

            if(!suite->suite_tests_container.empty()){
                for(auto& name: suite->suite_tests_container){
                    std::cout<<'\t'<<suite->name<<" function name: "<< name.get()->name<<'\n';
                }
            }
            else{
                std::cout<<"No tests in this suite.\n";
            }
            std::cout<<'\n';
        }
    }
}