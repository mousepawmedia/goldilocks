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
	using node_vector	= std::vector<std::unique_ptr<Node>>;
	using uniq_node		= std::unique_ptr<Node>;
	using uniq_test		= std::unique_ptr<Tests>;
	using uniq_suite	= std::unique_ptr<Suites>;

	// Vectors inside of the Suites class
	using suites_ctn    = std::vector<uniq_suite>;
	using tests_ctn     = std::vector<uniq_test>;

    /* This will allow all functions signatures to
    * be added to the Node, and not just specific types.*/
    using func          = std::function<void(void)>;

	// Used in Coordinator.hpp
	using _register = std::function<int(std::deque<std::string>&)>;
	using commands  = std::deque<std::string>&;

//    using container_of_suites = std::vector<std::shared_ptr<Suites>>;
//    using shr_suite = std::shared_ptr<Suites>;
//    using shr_runnable = std::shared_ptr<Runnable>;
//    using shr_test = std::shared_ptr<Tests>;
//    using suites_tests = std::map<std::string, std::shared_ptr<Runnable>>;
//    using shr_node = std::shared_ptr<Node>;
//    using shr_nodeptr = std::shared_ptr<Derived>;
//    using sent_node = std::pair<std::string_view, std::shared_ptr<Node>>;
//	using shr_suiteptr  = std::shared_ptr<Suites>;
//	using shr_testptr   = std::shared_ptr<Tests>;

};
#endif // ALIAS_HPP
