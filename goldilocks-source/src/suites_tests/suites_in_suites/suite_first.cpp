#include "goldilocks/coordinator/suites_tests/suite_first.hpp"

Suite_First::Suite_First():Suites{"Suite_First"}
{
    suites_container.emplace_back(std::make_shared<Suites>(suite1));
    suites_container.emplace_back(std::make_shared<Suites>(suite2));
}
