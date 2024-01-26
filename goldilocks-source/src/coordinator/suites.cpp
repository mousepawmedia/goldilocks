#include "goldilocks/coordinator/suites.hpp"

//Suites::Suites(std::string_view sent_name):Node{sent_name}, name{sent_name}
//{}

void Suites::add_suite(Alias::smrt_suite suite) {
	if (!has_child(std::string_view(suite->name))) {
		suites_container.emplace_back(std::move(suite));
	}
}

void Suites::add_test(Alias::smrt_test test) {
	if (!has_child(std::string_view(test->name))) {
		tests_container.emplace_back(std::move(test));
	}
}
