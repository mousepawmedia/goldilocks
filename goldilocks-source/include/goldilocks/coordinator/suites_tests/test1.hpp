#ifndef TEST1_HPP
#define TEST1_HPP

#include "goldilocks/coordinator/tests.hpp"

class Test1
{
public:
    Test1();
    virtual ~Test1() = default;

    std::vector<std::shared_ptr<Tests>> store_tests;

    const std::string                   name  {"Test1"};
    const std::string                   title {"Test1 Title"};
    const std::string                   docs  {"These are the docs"};

    const std::string&                  get_title() const;

    const std::string&                  get_docs() const;

    bool                                run_optimized() const;

    bool                                verify() const;

    bool                                pre() const;

    bool                                janitor() const;
};

#endif // TEST1_HPP
