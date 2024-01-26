#ifndef SUITE1_HPP
#define SUITE1_HPP

#include "goldilocks/coordinator/suites.hpp"
#include "goldilocks/coordinator/suites_tests/test1.hpp"
#include "goldilocks/coordinator/suites_tests/suite2.hpp"

class Suite1: public Suites
{
public:
    Suite1();
    virtual ~Suite1() = default;

    std::string name{"Suite1"};
    Test1 tests;
    Suite2 suite2;
};

#endif // SUITE1_HPP
