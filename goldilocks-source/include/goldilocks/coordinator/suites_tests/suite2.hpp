#ifndef SUITE2_HPP
#define SUITE2_HPP

#include "goldilocks/coordinator/suites.hpp"
#include "goldilocks/coordinator/suites_tests/test2.hpp"

class Suite2: public Suites
{
public:
    Suite2();
    virtual ~Suite2() = default;

    std::string name{"Suite2"};
    Test2 tests;
};

#endif // SUITE2_HPP
