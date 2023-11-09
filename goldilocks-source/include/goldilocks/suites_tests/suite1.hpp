#ifndef SUITE1_HPP
#define SUITE1_HPP

#include "suites.hpp"
#include "test1.hpp"
#include "suite2.hpp"

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
