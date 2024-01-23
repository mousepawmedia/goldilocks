#ifndef TESTS_HPP
#define TESTS_HPP

#include "goldilocks/coordinator/node.hpp"
#include "goldilocks/coordinator/runnable.hpp"
#include "goldilocks/coordinator/alias.hpp"

class Tests: private Node
{
public:
    Tests(std::string_view, Alias::func);
    virtual  ~Tests() = default;

	std::string_view    name;
	Alias::func			test;
};

#endif // TESTS_HPP
