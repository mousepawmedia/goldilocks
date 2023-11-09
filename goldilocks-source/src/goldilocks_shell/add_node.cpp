#include "coordinator.hpp"

//void Coordinator::add_node(std::shared_ptr<Suites> suite, Node<Suites>::shr_nodeptr node)
void Coordinator::add_node(shrptr_suite& suites, Node<Suites>::shr_nodeptr node)
{
    std::cout<<"add_node for suite: "<<suites->name<< " called on node: "<< node->name<<"\n";
//    node->children.emplace_back(std::make_shared<Node<Suites>>(suites->name));
    node->children.emplace_back(std::make_shared<Suites>(suites->name));
}

void Coordinator::add_node(std::shared_ptr<Tests> test, Node<Suites>::shr_nodeptr node)
{
    std::cout<<"add_node for test: "<<test->name<< " called on node: "<< node->name<<"\n";
//    node->children.emplace_back(std::make_shared<Node<Tests>>(test->name, test->test));
}
