#ifndef RUNNABLE_HPP
#define RUNNABLE_HPP

#include <iostream>
#include <functional>

/* This is the class all tests will have to inherit. */
class Runnable
{
public:

    /* This will allow all functions signatures to
     * be added to the Node, and not just specific types.*/
    using func = std::function<void(void)>;

    Runnable(std::string_view, func);
    virtual ~Runnable() = default;

    Runnable(const Runnable&) = delete;
    Runnable(Runnable&&) = delete;

    std::string_view name;
    func             test;
};

#endif // RUNNABLE_HPP
