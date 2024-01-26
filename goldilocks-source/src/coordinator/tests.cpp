#include "goldilocks/coordinator/tests.hpp"

Tests::Tests(std::string_view sent_name, Alias::func sent_func)
: Node{sent_name}, name{sent_name}, test{sent_name, sent_func}
{}
