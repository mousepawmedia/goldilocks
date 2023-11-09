#ifndef USING_HPP
#define USING_HPP

#include "goldilocks/runnable.hpp"
#include "goldilocks/node.hpp"
#include "goldilocks/suites_tests/suites.hpp"
#include "goldilocks/suites_tests/tests.hpp"

namespace{
    using container_of_suites = std::vector<std::shared_ptr<Suites>>;
    using shr_suite = std::shared_ptr<Suites>;
    using shr_runnable = std::shared_ptr<Runnable>;
    using shr_test = std::shared_ptr<Tests>;
    using suites_tests = std::map<std::string, std::shared_ptr<Runnable>>;
    using shr_node = std::shared_ptr<Node>;
    using sent_node = std::pair<std::string_view, std::shared_ptr<Node>>;
    using func = std::function<void(void)>;
}
#endif // !USING_HPP#define USING_HPP

namespace{

}