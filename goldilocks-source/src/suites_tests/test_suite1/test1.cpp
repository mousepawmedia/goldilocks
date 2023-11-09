#include "test1.hpp"

Test1::Test1()
{
    store_tests.emplace_back(std::make_shared<Tests>("get_title", [this]{get_title();}));
    store_tests.emplace_back(std::make_shared<Tests>("get_docs", [this]{get_docs();}));
    store_tests.emplace_back(std::make_shared<Tests>("run_optmized", [this]{run_optimized();}));
    store_tests.emplace_back(std::make_shared<Tests>("verify", [this]{verify();}));
    store_tests.emplace_back(std::make_shared<Tests>("pre", [this]{pre();}));
    store_tests.emplace_back(std::make_shared<Tests>("janitor", [this]{janitor();}));

}

const std::string& Test1::get_title() const {std::cout<< "Running get_title\n"; return title;}

const std::string& Test1::get_docs() const {std::cout<< "Running get_docs\n"; return docs;}

bool Test1::run_optimized() const {std::cout<< "Running run_optmmized\n"; return true;}

bool Test1::verify() const {std::cout<< "Running verify\n"; return true;}

bool Test1::pre() const {std::cout<< "Running pre\n"; return true;}

bool Test1::janitor() const {std::cout<< "Running janitor\n"; return true;    }
