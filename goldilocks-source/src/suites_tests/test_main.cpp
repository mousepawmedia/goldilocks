#include "goldilocks/coordinator/suites_tests/test_main.hpp"

//Is this even used?!?!?!?!
Test_Main::Test_Main(){
    suite1->suite_add_test(test1);
    suite1->suite_add_test(test2);
    suite1->suite_add_test(test3);

    suite2->suite_add_test(test1);
    suite2->suite_add_test(test2);
    suite2->suite_add_test(test3);

    suite3->suite_add_test(test1);
    suite3->suite_add_test(test2);
    suite3->suite_add_test(test3);

    suite_main->suite_add_suite(suite1);
    suite_main->suite_add_suite(suite2);
    suite_main->suite_add_suite(suite3);

    tests->children.push_back(std::make_shared<Tests>(this->name +" get_title", [this]{get_title();}));
    tests->children.push_back(std::make_shared<Tests>(this->name +" get_docs", [this]{get_docs();}));
    tests->children.push_back(std::make_shared<Tests>(this->name +" run_optimized", [this]{run_optimized();}));
    tests->children.push_back(std::make_shared<Tests>(this->name +" verify", [this]{verify();}));
    tests->children.push_back(std::make_shared<Tests>(this->name +" pre", [this]{pre();}));
    tests->children.push_back(std::make_shared<Tests>(this->name +" janitor", [this]{janitor();}));

    suite_main->suite_add_test(tests);

    suite1->print_suites(suite_main.get());
}
