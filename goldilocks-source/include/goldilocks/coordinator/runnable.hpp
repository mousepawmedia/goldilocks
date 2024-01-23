#ifndef RUNNABLE_HPP
#define RUNNABLE_HPP

#include <iostream>
//#include <functional>
#include "goldilocks/coordinator/alias.hpp"

/* This is the class all tests will have to inherit. */
class Runnable
{
public:

    Runnable(std::string_view, Alias::func);
    virtual ~Runnable() = default;

    Runnable(const Runnable&) = delete;
    Runnable(Runnable&&) = delete;

	std::string_view	name;
	Alias::func			test;
};

#endif // RUNNABLE_HPP
