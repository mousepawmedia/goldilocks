#ifndef SUITE_THIRD_HPP
#define SUITE_THIRD_HPP

#include "goldilocks/coordinator/suites.hpp"
#include "goldilocks/coordinator/suites_tests/suite1.hpp"
#include "goldilocks/coordinator/suites_tests/suite2.hpp"

class Suite_Third: private Suites
{
public:
    Suite_Third();
    virtual ~Suite_Third() = default;

    std::string name{"Suite_Third"};
    Suite1      suite1;
    Suite2      suite2;
};

#endif // SUITE_THIRD_HPP
