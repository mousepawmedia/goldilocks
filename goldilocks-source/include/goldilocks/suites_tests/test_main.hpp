#ifndef TEST_MAIN_HPP
#define TEST_MAIN_HPP


#include "goldilocks/suites_tests/test1.hpp"
#include "goldilocks/suites_tests/test2.hpp"
#include "goldilocks/suites_tests/test3.hpp"
#include "goldilocks/suites_tests/suites.hpp"

//Is this even used?!?!?!?!
class Test_Main
{
    const std::string name{"Test_Main"};

    std::shared_ptr<Test1> test1{std::make_shared<Test1>()};
    std::shared_ptr<Test2> test2{std::make_shared<Test2>()};
    std::shared_ptr<Test3> test3{std::make_shared<Test3>()};
    std::shared_ptr<Suites> suite_main{std::make_shared<Suites>(name)};
    std::shared_ptr<Suites> suite1{std::make_shared<Suites>("suite1")};
    std::shared_ptr<Suites> suite2{std::make_shared<Suites>("suite2")};
    std::shared_ptr<Suites> suite3{std::make_shared<Suites>("suite3")};

    Tests::shr_test tests{std::make_shared<Tests>(name)};

    const std::string title{"Test_Main Title"};
    const std::string docs{"These are the Test_Main docs"};

    const std::string& get_title() const { return title;}

    const std::string& get_docs() const { return docs;}

    bool run_optimized() const { return true;}

    bool verify() const { return true;}

    bool pre() const { return true;}

    bool janitor() const { return true;}
    std::string test_name{"Testing"};

public:
    Test_Main();
    // ~Test_Main() = default;
    virtual ~Test_Main() = default; //{std::cout<< "Test_Main destructor called\n";}


};

#endif // TEST_MAIN_HPP
