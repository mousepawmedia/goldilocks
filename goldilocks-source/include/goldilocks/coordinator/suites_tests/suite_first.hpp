#ifndef SUITE_FIRST_HPP
#define SUITE_FIRST_HPP

#include "goldilocks/coordinator/suites.hpp"
#include "suite1.hpp"
#include "suite2.hpp"

class Suite_First: public Suites
{
public:
    Suite_First();
    virtual ~Suite_First() = default;
    std::string name{"Suite_First"};
    Suite1      suite1;
    Suite2      suite2;
};

#endif // SUITE_FIRST_HPP
