#include "goldilocks/suites_tests/suite_main.hpp"

Suite_Main::Suite_Main(): Suites("Suite_Main")
{
    //Adding a test via void add_test(const std::string, std::shared_ptr<Runnable>)
    this->test->add_test("get_docs", docs);
    this->test->add_test("optimized", optimized);
    this->test->add_test("verify", verify);
    this->test->add_test("pre", pre);
    this->test->add_test("janitor", janitor);

    this->test1->add_test("get_docs", [this]{test1->get_docs();});
    this->test1->add_test("get_docs", [this]{test1->get_title();});
    this->test1->add_test("get_docs", [this]{test1->run_tests();});
    this->test1->add_test("get_docs", [this]{test1->run_optimized();});
    this->test1->add_test("get_docs", [this]{test1->pre();});
    this->test1->add_test("get_docs", [this]{test1->verify();});
    this->test1->add_test("get_docs", [this]{test1->janitor();});

    this->test2->add_test("get_docs", [this]{test2->get_docs();});
    this->test2->add_test("get_docs", [this]{test2->get_title();});
    this->test2->add_test("get_docs", [this]{test2->run_tests();});
    this->test2->add_test("get_docs", [this]{test2->run_optimized();});
    this->test2->add_test("get_docs", [this]{test2->pre();});
    this->test2->add_test("get_docs", [this]{test2->verify();});
    this->test2->add_test("get_docs", [this]{test2->janitor();});

    this->multi_first->suite_add_test(test1);
    this->multi_first->suite_add_test(test2);
    this->multi_first->suite_add_test(test);

    this->multi_second->suite_add_test(test1);
    this->multi_second->suite_add_test(test2);
    this->multi_second->suite_add_test(test);
    this->multi_first->suite_add_suite(multi_second);

    this->multi_third->suite_add_test(test);
    this->multi_third->suite_add_test(test2);
    this->multi_third->suite_add_test(test1);

    this->multi_suite->suite_add_suite(multi_first);
    this->multi_suite->suite_add_suite(multi_second);
    this->multi_suite->suite_add_suite(multi_third);

    this->suite_add_test(test1);
    this->suite_add_test(test2);
    this->suite_add_suite(multi_suite);
    this->suite_add_suite(multi_first);
    this->suite_add_suite(multi_second);
    this->suite_add_suite(multi_third);

    this->print_suites(multi_suite.get());
//    for(auto& suite: this->send_suites()){
//        for(auto& other: suite->send_suites()){
//            std::cout<<other->name<<" inside container "<< suite->name<<'\n';
//        }
//    }

}
