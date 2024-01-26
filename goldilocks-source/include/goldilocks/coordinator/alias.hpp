#ifndef ALIAS_HPP
#define ALIAS_HPP

#include <functional>
#include <memory>
#include <vector>
#include <deque>

/* Forward declare any classes using the alias header which
 * also will need to be used here in alias.hpp to avoid
 * circular dependancies. */
class Node;
class Runnable;
class Tests;
class Suites;

class Alias{
public:

	// Smart pointers for the containers.
	using node_vector	= std::vector<std::unique_ptr<Node>>;
	using smrt_node		= std::unique_ptr<Node>;
	using smrt_test		= std::unique_ptr<Tests>;
	using smrt_suite	= std::unique_ptr<Suites>;

	// Vectors inside of the Suites class
	using suites_ctn    = std::vector<smrt_suite>;
	using tests_ctn     = std::vector<smrt_test>;

    /* This will allow all functions signatures to
    * be added to the Node, and not just specific types.*/
    using func          = std::function<void(void)>;

	// Used in Coordinator.hpp
	using _register		= std::function<int(std::deque<std::string>&)>;
	using commands		= std::deque<std::string>&;
};
#endif // ALIAS_HPP
