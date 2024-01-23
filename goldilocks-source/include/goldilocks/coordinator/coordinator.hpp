#ifndef COORDINATOR_HPP
#define COORDINATOR_HPP

#include "goldilocks/coordinator/node.hpp"
#include "goldilocks/coordinator/suites_tests/test_list.hpp"
#include "iosqueak/channel.hpp"
#include <deque>

class Coordinator
{
public:

    Coordinator();
    virtual ~Coordinator() = default;

    /* Root node for the tree. */
    Alias::uniq_node   root{std::make_unique<Suites>("root")};


	/* Add node to the tree. */
	void add_node(Alias::uniq_suite&, Alias::uniq_node);
	void add_node(std::unique_ptr<Tests> test, Alias::uniq_node node);

    /* Program to find if a node is already in the tree. */
    Alias::uniq_node    find_node(std::string_view, Alias::uniq_node);
    Alias::uniq_node    find_node(std::deque<std::string>&, Alias::uniq_node);
    Alias::uniq_test    find_node(std::string_view, Alias::uniq_test);

    /* Program to initially fill the tree with the Tests
     * or Suites added by the programmer. */
    void fill_tree(const std::vector<std::unique_ptr<Node>>&, std::unique_ptr<Suites>&);

    /* Temporary(?) program to print the tree to check if
     * tree was filled successfully. */
    void print_tree(const Alias::uniq_node&, int depth = 0);

	// Process deque<string> to check for sent tests.
	Alias::commands process_tests(Alias::commands& tests);

	/* Program to process tests in Suites container. */
	void process_test_container(Alias::uniq_suite);

	bool add_suite(std::shared_ptr<Node> runner);
};

#endif // COORDINATOR_HPP

//    void fill_tree(std::string_view, Suites::suites_ctn&, Suites::shr_suite);
//    void fill_tree(std::string_view, Suites::suites_ctn&, Node::shr_nodeptr);
//    void fill_tree(std::string_view, Node<NodeBase>);
//    void print_tree(Node<NodeBase>);
//    void fill_tree(std::string_view, Node::shr_nodeptr);
//    void fill_tree(Suites::tests_ctn&,  Node::shr_nodeptr);
//    void add_node(std::shared_ptr<Suites>, Node::shr_nodeptr);
//    Node::shr_nodeptr   root{std::make_shared<Node>("root")};
