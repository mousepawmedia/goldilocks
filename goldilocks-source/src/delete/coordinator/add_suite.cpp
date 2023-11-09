#include "../include/goldilocks/coordinator.hpp"

bool Coordinator::add_suite(std::shared_ptr<Node> runner)
{
    std::cout<<this->name <<" was sent to add_suite\n";
    std::cout<<"add_suite typeid: "<< typeid(runner).name()<<'\n';

    /* Checks if the runner sent is a Test or a Suite. */
    if(dynamic_cast<Tests*>(runner.get()) != nullptr){
        // Cast to Tests* as it is a Tests derived class.
        // Coordinator::available_tests[runner->node_name] = std::make_unique<Tests>(static_cast<Tests*>(&runner->node_name));
        // this->available_tests[runner->node_name] = (std::make_shared<Tests>(static_cast<Tests*>(runner.get())->node_name));
        std::cout<<"Adding test: "<< runner->node_name <<'\n'<<"test size: "<< this->available_tests.size()<<'\n';
        return true;
    }

    else if(dynamic_cast<Suites*>(runner.get()) != nullptr){
        // Cast to Suites* as it is a Suites derived class.
        // this->available_suites[runner->node_name] = std::make_shared<Suites>(static_cast<Suites*>(runner.get())->node_name);
        std::cout<<"Adding suite: "<< runner->node_name <<'\n'<<"suites size: "<< this->available_suites.size()<<'\n';
        return true;
    }
    else{
        std::cout<<runner->node_name << " is not a Suite or Test\n";
        return false;
    }
    
        std::cout<<"Coordinator available_suite size: "<<this->available_suites.size()<<'\n'<< "available_tests size: " << this->available_tests.size()<<'\n';

    //Delete when adding suites if working.
    // for(auto& [key, value]: Coordinator::available_suites){
    //     for(auto& test: value.get()){
    //         std::cout<<key<<" with suite "<<test->name<< " was added to coordinator suite\n";
    //     }
    // }
    // return true;
}