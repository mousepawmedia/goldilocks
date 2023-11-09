#include "suite_third.hpp"

Suite_Third::Suite_Third(): Suites{"Suite_Third"}
{
    for(auto& test: suite1.tests.store_tests)
    {
        this->tests_container.push_back(std::make_shared<Tests>(test->name, test->test));
    }

    for(auto& test: suite2.tests.store_tests)
    {
        this->tests_container.push_back(std::make_shared<Tests>(test->name, test->test));
    }
}
