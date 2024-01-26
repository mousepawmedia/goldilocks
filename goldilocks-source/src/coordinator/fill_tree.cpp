#include "goldilocks/coordinator/coordinator.hpp"

// Function to add test_container elements to tree.
void process_test_container(Alias::uniq_suite node);

// Temporary function to process string into vector. Delete when done.
std::deque<std::string> process_path(std::string& path);

void Coordinator::fill_tree(const std::vector<std::unique_ptr<Node>>& container, std::unique_ptr<Suites>& parent) {
    for (const auto& item : container) {
        if (std::unique_ptr<Suites> suite = std::dynamic_pointer_cast<Suites>(item)) {
            parent->add_suite(suite);
            auto test{dynamic_cast<Suites>(item)};
            fill_tree(suite, suite);
            fill_tree(suite->, suite);
        } else if (std::shared_ptr<Tests> test = std::dynamic_pointer_cast<Tests>(item)) {
            parent->add_test(test);
        }
    }
}

// Delete after? Use Stringify (I think).
std::deque<std::string> process_path(std::string& path)
{
	std::deque<std::string>     vec;
	std::string                 word;

	std::for_each(path.begin(), path.end(), [&vec, &word](char ch)
				  {
					  if(ch != '.') word.push_back(ch);
					  else{
						  vec.emplace_back(word);
						  word = std::string();
					  }
				  });

	vec.emplace_back(word);

	return vec;
}

