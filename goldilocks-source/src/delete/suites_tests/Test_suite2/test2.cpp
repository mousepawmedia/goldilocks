#include "goldilocks/suites_tests/test2.hpp"

Test2::Test2(): Tests("Test2")
{
    Tests::add_test(this->name + "run_tests", [this]{run_tests();});
    Tests::add_test(this->name + "get_title", [this]{get_title();});
    Tests::add_test(this->name + "get_docs", [this]{get_docs();});
    Tests::add_test(this->name + "run_optimized", [this]{run_optimized();});
    Tests::add_test(this->name + "verify", [this]{verify();});
    Tests::add_test(this->name + "pre", [this]{pre();});
    Tests::add_test(this->name + "janitor", [this]{janitor();});
}

void Test2::run_tests()
{
    std::cout<<"Running Test2\n";
}

const std::string& Test2::get_title() const { return title;}

const std::string& Test2::get_docs() const { return docs;}

bool Test2::run_optimized() const { return true;}

bool Test2::verify() const { return true;}

bool Test2::pre() const { return true;}

bool Test2::janitor() const { return true;    }
