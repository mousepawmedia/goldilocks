#include "../include/goldilocks/coordinator.hpp"

Coordinator::Coordinator()
{   
    /* Add all of the tests in Test_List class to the
     * proper containers via add_suite function. */ 
    // auto testing{testlist.list_tests()};
    // for(auto& test: testing)
    // {   
    //     Coordinator::add_suite(test.get);
    // }
 
    std::cout<<"Coordinator start suite sizes: "<< this->available_suites.size()<<" and " << this->available_tests.size()<<'\n'; 
}

Coordinator::store_suites& Coordinator::send_suites()
{
    return available_suites;
}
Coordinator::store_tests& Coordinator::send_tests()
{
    return available_tests;
}

// bool Coordinator::is_suite(const std::string& name){
//     if()
// }

