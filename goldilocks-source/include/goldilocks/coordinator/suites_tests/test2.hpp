#ifndef TEST2_HPP
#define TEST2_HPP


#include "goldilocks/coordinator/tests.hpp"

class Test2
{
public:
    Test2();
    virtual ~Test2() = default;

    std::vector<std::shared_ptr<Tests>> store_tests;

    const std::string                   name  {"Test2"};
    const std::string                   title {"Test2 Title"};
    const std::string                   docs  {"These are the docs"};

    const std::string&                  get_title() const;

    const std::string&                  get_docs() const;

    bool                                run_optimized() const;

    bool                                verify() const;

    bool                                pre() const;

    bool                                janitor() const;
};

#endif // TEST2_HPP
