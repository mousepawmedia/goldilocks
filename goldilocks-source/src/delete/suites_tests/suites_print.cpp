#include "goldilocks/suites_tests/suites.hpp"

// void Suites::print_container(std::shared_ptr<Suites> suites){
void Suites::print_container(Suites* suites){
    // std::cout<<"Inside "<< suites->node_name <<" print_container. Suite size: "<<suites->suites_container.size()<<'\n';
    // if (suites_container.empty() && suite_tests_container.empty()) return;

    if(!suites->suites_container.empty()){

        for(auto& suite: suites->suites_container){
            // std::cout << i <<" Suite "<< suite->suite_name <<":\n";
            std::cout <<"Suite "<< suite->name <<" contains:\n";
            std::cout<< suite->name <<" suite continer size is: "<< suite->suites_container.size()<<'\n';

            if(!suite->suite_tests_container.empty()){
                for(auto& name: suite->suite_tests_container){
                    std::cout<<'\t'<<suite->name<<" function name: "<< name.first<<'\n';
                }
            }
            else{
                std::cout<<"No tests in this suite.\n";
            }
            std::cout<<'\n';
        }
    }
}