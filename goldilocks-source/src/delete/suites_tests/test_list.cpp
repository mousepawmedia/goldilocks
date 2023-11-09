#include "goldilocks/suites_tests/test_list.hpp"

Test_List::Test_List()
{
    Test_List::add_tests();
    std::cout<<"list_of_tests size: "<<list_of_tests.size()<<'\n';
    // Single tests:
    // check if suite or test. Add to appropriate map.
    // How to call up suite1.suite1.test etc?

} 

void Test_List::add_tests()
{
    this->list_of_tests.push_back(std::make_shared<Node>(test1->name));
    this->list_of_tests.push_back(std::make_shared<Node>(test2->name));
    this->list_of_tests.push_back(std::make_shared<Node>(test3->name));
    this->list_of_tests.push_back(std::make_shared<Node>(suite_main->name));
}

tests& Test_List::list_tests()
{
    return list_of_tests;
}
