#include "goldilocks/suites_tests/test1.hpp"

Test1::Test1(): Tests("Test1")
{
    Tests::add_test(this->node_name + "run_tests", [this]{run_tests();});
    Tests::add_test(this->node_name + "get_title", [this]{get_title();});
    Tests::add_test(this->node_name + "get_docs", [this]{get_docs();});
    Tests::add_test(this->node_name + "run_optimized", [this]{run_optimized();});
    Tests::add_test(this->node_name + "verify", [this]{verify();});
    Tests::add_test(this->node_name + "pre", [this]{pre();});
    Tests::add_test(this->node_name + "janitor", [this]{janitor();});
}

void Test1::run_tests()
{
    std::cout<<"Running Test1\n";
}

const std::string& Test1::get_title() const { return title;}

const std::string& Test1::get_docs() const { return docs;}

bool Test1::run_optimized() const { return true;}

bool Test1::verify() const { return true;}

bool Test1::pre() const { return true;}

bool Test1::janitor() const { return true;    }
