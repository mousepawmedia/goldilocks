#include "goldilocks/suites_tests/tests.hpp"

Tests::Tests(std::string_view name): Node(name), name{name}
{

//     std::cout<<name << " test constructor called\n";
}

void Tests::add_test(std::string name, Runnable::func funct){

    this->tests_container[name] = std::make_shared<Runnable>(this->name, funct);
//        std::cout<<"Tests add_test container: "<<this->name<<" test size: "<< this->tests_container.size()<<'\n';
}

void Tests::add_test(const std::string name, std::shared_ptr<Runnable>runner)
{
    this->tests_container[name] = runner;
}

void Tests::printTests(const Tests& sent_test) const
{
    std::cout<<"Tests in " <<this->node_name<<":\n";
    for(auto& tests: sent_test.tests_container){
        std::cout<<"Test "<< tests.first <<'\n';
    }
}

Tests::suites_tests& Tests::send_test_container(){
    return tests_container;
}
