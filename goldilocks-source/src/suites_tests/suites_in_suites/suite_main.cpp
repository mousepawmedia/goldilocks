#include "suite_main.hpp"

Suite_Main::Suite_Main(): Suites{"Suites_Main"}
{
    suites_container.emplace_back(std::make_shared<Suites>(suite_first));
    suites_container.emplace_back(std::make_shared<Suites>(suite_second));

    for(auto& test: test1.store_tests)
    {
        tests_container.emplace_back(std::make_shared<Tests>(test->name, test->test));
    }
}
