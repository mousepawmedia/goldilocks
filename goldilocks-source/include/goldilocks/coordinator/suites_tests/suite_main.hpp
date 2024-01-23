#ifndef SUITE_MAIN_HPP
#define SUITE_MAIN_HPP

#include "goldilocks/coordinator/suites.hpp"
#include "suite_second.hpp"
#include "suite_first.hpp"
#include "test1.hpp"

class Suite_Main: public Suites
{
public:
    Suite_Main();
    virtual ~Suite_Main() = default;

    std::string     name{"Suite_Main"};
    Suite_First     suite_first;
    Suite_Second    suite_second;
    Test1           test1;
};

#endif // SUITE_MAIN_HPP
