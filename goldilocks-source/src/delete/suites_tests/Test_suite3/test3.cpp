#include "goldilocks/suites_tests/test3.hpp"

Test3::Test3(): Tests("Test3")
{
    Tests::add_test(this->name + "get_title", [this]{get_title();});
    Tests::add_test(this->name + "get_docs", [this]{get_docs();});
    Tests::add_test(this->name + "run_optimized", [this]{run_optimized();});
    Tests::add_test(this->name + "verify", [this]{verify();});
    Tests::add_test(this->name + "pre", [this]{pre();});
    Tests::add_test(this->name + "janitor", [this]{janitor();});
}

const std::string& Test3::get_title() const { return title;}

const std::string& Test3::get_docs() const { return docs;}

bool Test3::run_optimized() const { return true;}

bool Test3::verify() const { return true;}

bool Test3::pre() const { return true;}

bool Test3::janitor() const { return true;}
