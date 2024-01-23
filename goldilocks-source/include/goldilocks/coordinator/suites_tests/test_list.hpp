#ifndef TEST_LIST_HPP
#define TEST_LIST_HPP

#include "goldilocks/coordinator/suites.hpp"
#include "goldilocks/coordinator/suites_tests/suite_main.hpp"

class Test_List
{
public:
    Test_List();
    virtual ~Test_List() = default;

    Suites      main_suite{"Test_List"};
    Suite_Main  test_suite;
};

#endif // TEST_LIST_HPP
