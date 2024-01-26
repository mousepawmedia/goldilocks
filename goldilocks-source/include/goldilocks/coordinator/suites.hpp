#ifndef SUITES_HPP
#define SUITES_HPP

#include "goldilocks/coordinator/node.hpp"
#include "goldilocks/coordinator/tests.hpp"

class Suites : public Node
{
public:

	Suites(std::string_view);
	virtual ~Suites() = default;

    std::string         name;
    Alias::suites_ctn   suites_container;
    Alias::tests_ctn    tests_container;

	void add_suite(Alias::smrt_suite);
	void add_test (Alias::smrt_test);

	void print_container(Suites* suites); //Needed?
};

#endif // SUITES_HPP
