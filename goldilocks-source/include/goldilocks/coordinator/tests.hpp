#ifndef TESTS_HPP
#define TESTS_HPP

#include "goldilocks/coordinator/node.hpp"
#include "goldilocks/coordinator/runnable.hpp"

class Tests: public Node
{
public:
    Tests(std::string_view, Alias::func);
    virtual  ~Tests() = default;

    std::string	name;
    Runnable	test;
};

#endif // TESTS_HPP
