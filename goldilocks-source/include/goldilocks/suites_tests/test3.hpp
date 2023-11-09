#ifndef TEST3_HPP
#define TEST3_HPP

#include "tests.hpp"

class Test3
{
public:
    Test3();
    virtual ~Test3() = default;

    std::vector<std::shared_ptr<Tests>> store_tests;

    const std::string                   name  {"Test3"};
    const std::string                   title {"Test3 Title"};
    const std::string                   docs  {"These are the docs"};

    const std::string&                  get_title() const;

    const std::string&                  get_docs() const;

    bool                                run_optimized() const;

    bool                                verify() const;

    bool                                pre() const;

    bool                                janitor() const;
};

#endif // TEST3_HPP
