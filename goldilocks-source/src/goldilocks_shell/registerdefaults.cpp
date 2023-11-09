#include "../include/goldilocks/goldilocks_shell.hpp"

void Goldilocks_Shell::register_defaults()
{
    Blueshell::register_command("about", std::bind(&Goldilocks_Shell::about, this, _1),"Display the documentation and other information for the indicated [item].", "Display the documentation and other information for the indicated [item]. An example is 'about calculator'", 1, 0);
    
    Blueshell::register_command("benchmark", std::bind(&Goldilocks_Shell::benchmark, this, _1),"Runs a benchmark on [test].", "Runs a benchmark on [test], using its registered comparative, with (number) repetitions.", 1 , 1);
    
    Blueshell::register_command("compare", std::bind(&Goldilocks_Shell::compare, this, _1),"Runs a comparative benchmark between [test1] and [test2] with (number) repetitions.", "Runs a comparative benchmark between [test1] and [test2] with (number) repetitions. This is seldom used, as its prone to apples-and-oranges comparisons, unlike benchmark, but it can be useful at times.", 2, 1);
    
    Blueshell::register_command("kamikaze", std::bind(&Goldilocks_Shell::kamikaze, this, _1),"Runs a [test] as a controlled crash, logging test details to an external file on the way down.", "Runs a [test] as a controlled crash, logging test details to an external file on the way down. This is the only way to run Fatality Tests, but is also useful for Stress Tests, Edge Tests, and Fuzz Tests when a crash is occurring.", 1, 0);
    
    Blueshell::register_command("list", std::bind(&Goldilocks_Shell::list, this, _1),"Displays the available suites.", "Displays the available suites. Passing an optional (suite) will display all the tests in that suite.", 0, 1);
        
    Blueshell::register_command("list_loaded", std::bind(&Goldilocks_Shell::list_loaded, this, _1),"Displays the available suites.", "Displays the available suites. Passing an optional (suite) will display all the tests in that suite.", 0, 1);
    
    Blueshell::register_command("run", std::bind(&Goldilocks_Shell::run, this, _1),"Runs an [item], being a test or suite.", "Runs an [item], being a test or suite, with the optional (number) of repetitions. The default is 1.", 1, 1);  
    
    Blueshell::register_command("find", std::bind(&Goldilocks_Shell::find, this, _1),
    "Checks if a test is loaded.", "Checks if a test is loaded after running 'load <test name>' to load the test. Call using find <test_name> (eg 'find test' or if further down the tree, find test.test1.test2 etc)", 1, 0);
    
    Blueshell::register_command("load", std::bind(&Goldilocks_Shell::load, this, _1), "Loads the tests to run.",
    "Loads the tests to run from the available commands.  Call using 'load <test_name>' (eg 'load test' or if loading from further down the tree, find test.test1.test2 etc)", 0, 1);
}
