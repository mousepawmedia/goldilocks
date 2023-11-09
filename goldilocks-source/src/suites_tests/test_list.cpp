#include "test_list.hpp"

Test_List::Test_List()
{
    main_suite.suites_container.emplace_back(std::make_shared<Suites>(test_suite.suite_first));
    main_suite.suites_container.emplace_back(std::make_shared<Suites>(test_suite.suite_second));

    std::cout<<"Adding Test_List tests.\n";
    for(auto& test: main_suite.tests_container)
    {
        main_suite.tests_container.emplace_back(std::make_shared<Tests>(test->name, test->test));
    }
}
