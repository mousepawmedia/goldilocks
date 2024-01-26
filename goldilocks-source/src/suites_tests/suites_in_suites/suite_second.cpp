#include "goldilocks/coordinator/suites_tests/suite_second.hpp"

Suite_Second::Suite_Second(): Suites{"Suite_Second"}
{
    this->suites_container.emplace_back(std::make_shared<Suites>(suite1));
    this->suites_container.emplace_back(std::make_shared<Suites>(suite2));
}
