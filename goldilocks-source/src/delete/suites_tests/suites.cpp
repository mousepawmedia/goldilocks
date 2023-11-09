#include "goldilocks/suites_tests/suites.hpp"

Suites::Suites(std::string_view name): Node(name), name{name}
{
    // name_function = func_rename<sent_test->test_>();
    // name = name_function.first;
    // test = name_function.second;
}

void Suites::suite_add_suite(std::shared_ptr<Suites> suite)
{
    this->suites_container.push_back(suite);
}

void Suites::suite_add_test(std::shared_ptr<Tests> test)
{

    for(auto& [key, value]: test->send_test_container()){
        this->suite_tests_container[key] = std::make_shared<Runnable>(key, value->test_);
    }
}

void Suites::print_suites(Suites* suites)
{
    std::cout<<suites->name<< " calling print_suites with container size: "<<suites_container.size()<<'\n';
    if(!suites->suites_container.empty())
    {
        for(auto& suite: suites->suites_container)
        {
            std::cout<<suite->name <<" is in the " << this->name << " container\n";
            Suites::print_suites(suite.get());
        }

    }

    if(!suites->suites_container.empty() && !suites->suite_tests_container.empty())
    {
        std::cout<<"Tests inside "<< suites->name <<":\n";
        for(auto& [key, value]: suites->suite_tests_container)
        {
            std::cout<<'\t'<< key << " is in the suites test container\n";
        }
    }

    if(!suites->suite_tests_container.empty() && suites->suites_container.empty())
    {
        std::cout<<"Tests inside "<< suites->name <<":\n";
        for(auto& [key, value]: suites->suite_tests_container)
        {
            std::cout<<'\t'<< key << " is in the suites test container\n";
        }
    }

}
