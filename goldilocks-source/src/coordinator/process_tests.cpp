#include "goldilocks/coordinator.hpp"
#include "iosqueak/stringy.hpp"
#include <sstream>

Coordinator::commands Coordinator::process_tests(commands& tests)
{
    /* A vector to copy the deque over to be processed
     * by stringy::split_string. */
    std::vector<std::string> sent_string;
    
    // Copy sent tests to sent_string vector for tokenizing.
    for_each(tests.begin(), tests.end(), [&sent_string](auto& word){
        sent_string.push_back(word);
    });
    
    // Sent vector to split_string for tokenizing.
   stringy::split_string(tests.front(), ".",sent_string);

    // Remove the original string from vector after tokenizing.
    sent_string.erase(sent_string.begin());

    // Clear sent tests container.
    tests.clear();
    
    // Add tokens to tests container.
    std::for_each(sent_string.begin(), sent_string.end(), [&tests](auto& word){tests.push_back(word);});
    
    return tests;
}
