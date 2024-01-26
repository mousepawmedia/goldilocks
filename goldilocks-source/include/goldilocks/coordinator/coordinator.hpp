#ifndef COORDINATOR_HPP
#define COORDINATOR_HPP

#include "goldilocks/coordinator/node.hpp"
#include "goldilocks/coordinator/suites.hpp"
#include "goldilocks/coordinator/suites_tests/test_list.hpp"
//#include "iosqueak/channel.hpp"
#include <deque>

class Coordinator
{
public:

    Coordinator();
    virtual ~Coordinator() = default;

    /* Root node for the tree. */
    Alias::smrt_node    root_node{std::make_unique<Suites>("root")};

	/* Program to find or add node to container. */
	void				add_find();

	/* Program to node to the container. Called by add_find. */
	void				add_node(Alias::smrt_suite, Node*);
	void				add_node(Alias::smrt_test, Node*);

    /* Program to find if a node is already in the tree. */
    Node*               find_node(Alias::commands, Node*);
    Node*               find_node(std::vector<std::string>, Node*);

    /* Program to initially fill the tree with the Tests
     * or Suites added by the programmer. */
    void                fill_tree(const Alias::node_vector&, Alias::smrt_suite&);

    /* Temporary(?) program to print the tree to check if
     * tree was filled successfully. */
    void                print_tree(const Alias::smrt_node&, int depth = 0);

	// Process deque<string> to check for sent tests.
	Alias::commands		process_tests(Alias::commands tests);

	/* Program to process tests in Suites container. */
	void				process_test_container(Alias::smrt_suite);

	bool				add_suite(std::shared_ptr<Node> runner);
};

#endif // COORDINATOR_HPP

