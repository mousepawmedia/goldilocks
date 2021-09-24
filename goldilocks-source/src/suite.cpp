#include "goldilocks/suite.hpp"

void TestSuite::register_item(itemname_t test_name, Test* test, Test* compare)
{
	//TODO Error Checking
	if (test == nullptr)
		throw std::invalid_argument("Test not valid");
	runnables.emplace(test_name, test);
	if (compare != nullptr)
		compares.emplace(test_name, compare);
}

void TestSuite::register_item(itemname_t item_name, TestSuite* suite)
{
	if (suite == nullptr)
		throw std::invalid_argument("Suite not valid");
	runnables.emplace(item_name, suite);
}