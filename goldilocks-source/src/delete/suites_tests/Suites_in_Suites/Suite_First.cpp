#include "goldilocks/suites_tests/suite_first.hpp"

Suite_First::Suite_First(): Suites("Suite_First")
{
    this->test1->add_test("get_docs", docs);
    this->test1->add_test("optimized", optimized);
    this->test1->add_test("verify", verify);
    this->test1->add_test("pre", pre);
    this->test1->add_test("janitor", janitor);
}
