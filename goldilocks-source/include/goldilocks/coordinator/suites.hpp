#ifndef SUITES_HPP
#define SUITES_HPP

#include "goldilocks/coordinator/alias.hpp"
#include "goldilocks/coordinator/node.hpp"
#include "goldilocks/coordinator/tests.hpp"

class Suites : public Node {
public:

	Suites(std::string_view);
	virtual ~Suites() = default;

    std::string         name;
    Alias::suites_ctn   suites_container;
    Alias::tests_ctn    tests_container;

	void add_suite(Alias::uniq_suite);
	void add_test (Alias::uniq_test);
};

#endif // SUITES_HPP
