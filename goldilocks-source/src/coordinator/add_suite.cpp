#include "goldilocks/coordinator.hpp"

bool Coordinator::add_suite(std::shared_ptr<Node> runner)
// bool Coordinator::add_suite(std::shared_ptr<Suites> runner)
{
    // std::cout<<this->name <<" was sent to add_suite\n";
    // // std::cout<<"add_suite typeid: "<< typeid(runner).name()<<'\n';
    // Node* node_type{runner.get()};
    // std::cout<<"add_suite type is: "<< typeid(node_type).name() <<'\n';
    (void) runner;
    return true;
}
