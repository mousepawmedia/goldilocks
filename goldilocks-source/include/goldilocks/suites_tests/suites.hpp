#ifndef SUITES_HPP
#define SUITES_HPP

#include "goldilocks/node.hpp"
#include "tests.hpp"

class Suites : public Node<Suites> {
public:
    using shr_suiteptr = std::shared_ptr<Suites>;
    using shr_testptr = std::shared_ptr<Tests>;

    Suites(std::string_view sent_name); // : Node<Suites>(sent_name) {}

    std::string name;

    void add_suite(shr_suiteptr suite) {
        if (!has_child(std::string(suite->name))) {
            add_child(suite);
        }
    }

    void add_test(shr_testptr test) {
        if (!has_child(std::string(test->name))) {
//            add_child(test);
        }
    }
};

//class Suites: public Node<Suites>
//{
//private:

//public:
//    using shr_suite     = std::shared_ptr<Suites>;
//    using suites_ctn    = std::vector<shr_suite>;
//    using tests_ctn     = std::vector<Tests::shr_test>;

//    Suites(std::string_view);
//    virtual ~Suites() = default;

//    std::string_view    name;
//    suites_ctn          suites_container;
//    tests_ctn           tests_container;
//};

#endif // SUITES_HPP
