#ifndef SUITE_THIRD_HPP
#define SUITE_THIRD_HPP

#include "suites.hpp"
#include "suite1.hpp"
#include "suite2.hpp"

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
