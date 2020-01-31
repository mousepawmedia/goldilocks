/** Manager [Goldilocks]
 * Version: 2.0
 *
 * TestManager stores and calls tests by names. It exposes functions for
 * testing and benchmarking, and automatically handles the proper call
 * sequence for any Test object. It also handles garbage collection for
 * all tests under its control.
 *
 * Author(s): Jason C. McDonald
 */

/* LICENSE (BSD-3-Clause)
 * Copyright (c) 2016-2019 MousePaw Media.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * CONTRIBUTING
 * See https://www.mousepawmedia.com/developers for information
 * on how to contribute to our projects.
 */

#ifndef GOLDILOCKS_MANAGER_HPP
#define GOLDILOCKS_MANAGER_HPP

#include <stdexcept>
#include <cstdlib>  // uint64_t and friends
#include <memory>  // std::unique_ptr
#include <map>  // std::map
#include <vector>  // std::vector

#include "iosqueak/stringy.hpp"  // String Manipulation
#include "iosqueak/channel.hpp"  // Output
#include "pawlib/pawsort.hpp"  // Sorting

#include "goldilocks/assertions.hpp"
#include "goldilocks/benchmark.hpp"
#include "goldilocks/benchmark_result.hpp"
#include "goldilocks/suite.hpp"
#include "goldilocks/test.hpp"
#include "goldilocks/types.hpp"

class Test;
class Suite;

class TestManager
{
protected:
    /** Stores all of the Suite pointers for access-by-name-string. */
    std::map<suitename_t, suiteptr_t> suites;

    /* We are using std::map intentionally above. Dynamic allocation is
     * more appropriate in this situation, especially since test
     * registration should be on-demand and front-loaded (all at once).*/
public:
    /* We are using smart pointers, so that TestManager exclusively
        * owns all test instances it uses, and thus can delete them
        * automatically later.*/

    /**The TestManager doesn't need anything to its constructor,
     * as all of its tests will be added ("registered") after the
     * fact, and it doesn't do any heap allocation besides that.*/
    TestManager() = default;

    /**List all tests registered with the TestManager.
     * \param whether to show the titles
     */
    void list_tests(bool showTitles = true)
    {
        /* Loop through all the indexes in the map `tests`...
         * SOURCE: http://stackoverflow.com/a/110255/472647
         */
        for(std::map<testname_t, testptr_t>::iterator it = tests.begin(); it != tests.end(); ++it)
        {
            // Print out the index to Channel.
            out << it->first;
            // If we're supposed to also display the title...
            if(showTitles)
            {
                // Show the title.
                out << ": " << it->second->get_title();
            }
            // Newline.
            out << IOCtrl::n;
        }
        out << IOCtrl::endl;
    }

    /**List all suites registered with the TestManager.
     * \param whether to show the titles
     */
    void list_suites(bool showTitles = true)
    {
        /* Loop through all the indexes in the map `suites`...
            * SOURCE: http://stackoverflow.com/a/110255/472647
            */
        for(std::map<suitename_t, suiteptr_t>::iterator it = suites.begin(); it != suites.end(); ++it)
        {
            // Print out the index to Channel.
            out << it->first;
            // If we're supposed to also display the title...
            if(showTitles)
            {
                // Show the title.
                out << ": " << it->second->get_title();
            }
            // Newline.
            out << IOCtrl::n;
        }
        out << IOCtrl::endl;
    }

    /** Load the tests in a test suite.
     * \param the name of the suite to load (load all if unspecified)
     * \return true if loaded, else false */
    bool load_suite(suitename_t suite = "")
    {
        /* ENTRY: Run a suite of tests.*/

        // If no suite is specified, load all available.
        if(suite == "")
        {
            /* Loop through all the indexes in the map `suites`...
                * SOURCE: http://stackoverflow.com/a/110255/472647
                */
            for(std::map<suitename_t, suiteptr_t>::iterator it = suites.begin(); it != suites.end(); ++it)
            {
                // Load each suite that isn't already loaded.
                if(it->second->is_loaded() == false)
                {
                    it->second->load_tests();
                    it->second->is_loaded(true);
                    out << IOCat::normal << IOFormatTextAttr::bold << it->first << ": " << it->second->get_title() << " loaded." << IOCtrl::endl;
                }
            }
            return true;
        }

        // Otherwise, if we specified a test...

        // Ensure the suite exists before continuing.
        /* NOTE: If you're getting a segfault in this function, check here!
            * Attempting to access the functions in an unregistered test causes
            * Undefined Behavior, typically a segfault. */
        if(!validate(suite, true, GolidlocksItemType::suite))
        {
            return false;
        }

        // Ensure the suite isn't already loaded...
        if(!suites[suite]->is_loaded())
        {
            // Load the suite into the testmanager.
            suites[suite]->load_tests();
            // Mark the suite as loaded.
            suites[suite]->is_loaded(true);
            // Report the good news.
            out << IOCat::normal << IOFormatTextAttr::bold << suites[suite]->get_title() << IOCtrl::endl;
            out << IOCat::normal << IOFormatTextAttr::bold << IOFormatTextBG::green << IOFormatTextFG::white
                << "Suite loaded." << IOCtrl::n << IOCtrl::endl;
            return true;
        }
        else
        {
            out << IOCat::error << IOFormatTextAttr::bold << IOFormatTextFG::red << "ERROR: The suite " << suite
                << " is already loaded."
                << IOCtrl::endl;
            return false;
        }
        return false;
    }

    /**Get the documentation string for a given test.
     * \param the test to return the documentation string for
     * \return the documentation string.
     */
    void show_docs(testname_t test_name)
    {
        // Ensure the test DOES NOT already exist before continuing.
        if(!validate(test_name, true, GolidlocksItemType::test))
        {
            return;
        }
        out << IOFormatTextAttr::bold << tests[test_name]->get_title()
            << "[" << test_name << "]: " << IOCtrl::end;
        out << tests[test_name]->get_docs() << IOCtrl::endl;
    }

    /**Register a new test with the TestManager.
     * \param a string of the test's name
     * \param a pointer to the test. The safest way to use this is to
     * pass the "new Test" as this argument, since TestManager will be
     * taking exclusive ownership of the instance. It will handle
     * NULL automatically, so no error checking is required.
     * \param a pointer to the comparative test. See above.*/
    void register_test(testname_t test_name, Test* test, Test* compare = 0)
    {
        /* ENTRY: Add a new test by name to the test manager. */

        // Ensure the test DOES NOT already exists before continuing.
        // If it DOES...
        if(validate(test_name, false, GolidlocksItemType::test))
        {
            // Throw an error.
            out << IOCat::error << IOFormatTextAttr::bold << IOFormatTextFG::red
                << "ERROR: A test by the name of " << test_name
                << "is already registered in Golidlocks Test Manager." << IOCtrl::endl;
            return;
        }

        /* We generally advise the end-developer to pass `new Test` as
            * the second parameter. Thus, if the allocation fails (or they
            * otherwise pass in a null pointer)...*/
        if(test == nullptr)
        {
            //Display an error message.
            out << IOCat::error << "TestManager: Unable to create a new "
                << "instance of the test \"" << test_name << "\". Test not "
                << "registered." << IOCtrl::endl;

            //Return non-fatally.
            return;
        }

        /* Add the new test to the TestManager's map (tests), with the name
            * string as the key, and a smart pointer (unique_ptr) to the test.
            * emplace() allows us to define the new unique_ptr within the map,
            * as insert() would literally NOT work (you can't copy a unique_ptr).*/
        tests.emplace(test_name, testptr_t(test));

        /* If a comparative test was included... */
        if(compare != nullptr)
        {
            /** Add the comparative test to the comparatives map using the
                 * test name as the key.
                 */
            comparatives.emplace(test_name, testptr_t(compare));
        }

        /* WARNING: The end-developer must be sure they aren't trying to
            * retain ownership of the Test instance, as that will cause UB
            * (in my observation, usually a segfault) when the TestManager
            * instance is destroyed - the unique_ptr will not be able to
            * destroy the instance. It's not pretty.*/
    }

    /**Register a new suite with the TestManager.
     * \param a string of the suite's name
     * \param a pointer to the suite. Follows the same rules as
     * those for `register_test()`.*/
    void register_suite(suitename_t suite_name, Suite* suite)
    {
        /* ENTRY: Add a new test by name to the test manager. */

        // Tell the suite instance which testmanager instance is controlling it.
        suite->backregister(this);

        // Ensure the suite DOES NOT already exist before continuing.
        // If it does...
        if(validate(suite_name, false, GolidlocksItemType::suite))
        {
            // Throw an error.
            out << IOCat::error << IOFormatTextAttr::bold << IOFormatTextFG::red
                << "ERROR: A suite by the name of " << suite_name
                << " is already registered in Golidlocks Test Manager." << IOCtrl::endl;
            return;
        }

        /* We generally advise the end-developer to pass `new Test` as
            * the second parameter. Thus, if the allocation fails (or they
            * otherwise pass in a null pointer)...*/
        if(suite == nullptr)
        {
            //Display an error message.
            out << IOCat::error << "TestManager: Unable to create a new "
                << "instance of the suite \"" << suite_name << "\". Suite not "
                << "registered." << IOCtrl::endl;

            //Return non-fatally.
            return;
        }

        /* Add the new suite to the TestManager's map (suites), with the name
            * string as the key, and a smart pointer (unique_ptr) to the suite.
            * emplace() allows us to define the new unique_ptr within the map,
            * as insert() would literally NOT work (you can't copy a unique_ptr).*/
        suites.emplace(suite_name, suiteptr_t(suite));

        /* WARNING: The end-developer must be sure they aren't trying to
            * retain ownership of the Suite instance, as that will cause UB
            * (in my observation, usually a segfault) when the TestManager
            * instance is destroyed - the unique_ptr will not be able to
            * destroy the instance. It's not pretty.*/
    }

    /** Run a test or suite intelligently.
         * \param the name of the item to run
         * \param the number of times to repeat the test (ignored on suites)
         * \return true if the item ran successfully, else false */
    bool run(testname_t item, unsigned int repeat = 1)
    {
        // Don't make this yell on fail...let the next conditional do it!
        if(validate(item, false, GolidlocksItemType::test))
        {
            return run_test(item, repeat);
        }
        else if(validate(item, true, GolidlocksItemType::suite))
        {
            return run_suite(item);
        }
        // If the item doesn't exist, return false.
        return false;
    }

    /**Run a test by name.
     * \param the name of the test to run
     * \param the number of times to repeat the test
     * \return true if the test ran successfully, else false */
    bool run_test(testname_t test, unsigned int repeat = 1)
    {
        /* ENTRY: Run a single test.*/

        // Ensure the test exists before continuing.
        /* NOTE: If you're getting a segfault in this function, check here!
            * Attempting to access the functions in an unregistered test causes
            * Undefined Behavior, typically a segfault. */
        if(!validate(test, true, GolidlocksItemType::test))
        {
            return false;
        }

        /* We should only allow repeating between 1 and 10,000 times,
            * inclusively. If the user asked for more than that...*/
        if(repeat < 1 || repeat > 10000)
        {
            // Display an error.
            out << IOCat::error << "Test repeat value must be between 1 and 10000 inclusively." << IOCtrl::endl;

            // Abort the test.
            return false;
        }

        // Display the test name in a banner.
        out << "===== [" << tests[test]->get_title() << "] =====" << IOCtrl::endl;

        /* Attempt to run the pretest function, which is intended to set up
            * for multiple runs of the test. If that fails (returns false)...*/
        if(!(tests[test]->pre()))
        {
            // Alert the user with an error message.
            out << IOCat::error << IOFormatTextAttr::bold << IOFormatTextBG::red << IOFormatTextFG::white
                << "PRE-TEST FAILED - ABORTING" << IOCtrl::endl;

            // Run the function to clean up after a pretest fail.
            tests[test]->prefail();

            // End of test.
            return false;
        }

        bool status = true;

        for(unsigned int i = 0; i < repeat; ++i)
        {
            out << IOCat::normal << "Pass " << stringy::itos(i+1)
                << " of " << stringy::itos(repeat) << IOCtrl::endl;

            tests[test]->janitor();

            // Run the test. If it fails (returned false)...
            if(!(tests[test]->run()) || !(tests[test]->verify()))
            {
                status = false;
                break;
            }
        }
        // If the test failed...
        if(!status)
        {
            // Alert the user with an error message.
            out << IOCat::error << IOFormatTextAttr::bold << IOFormatTextBG::red << IOFormatTextFG::white
                << "TEST FAILED" << IOCtrl::endl;

            // Run the test's fail-cleanup function.
            tests[test]->postmortem();

            //End of test.
            return false;
        }
        //If the test run succeeds (return true)...
        else
        {
            // Let the user know that all went well.
            out << IOCat::normal << IOFormatTextAttr::bold << IOFormatTextBG::green << IOFormatTextFG::white
                << "TEST COMPLETE" << IOCtrl::endl;

            // Run the test's cleanup (post) function.
            tests[test]->post();

            //End of test.
            return true;
        }
    }

    /**Run a suite by name.
     * \param the name of the suite to run
     * \return true if the suite ran successfully, else false */
    bool run_suite(suitename_t suite)
    {
        /* ENTRY: Run a suite of tests.*/

        // Ensure the suite exists before continuing.
        /* NOTE: If you're getting a segfault in this function, check here!
            * Attempting to access the functions in an unregistered test causes
            * Undefined Behavior, typically a segfault. */
        if(!validate(suite, true, GolidlocksItemType::suite))
        {
            return false;
        }

        if(!suites[suite]->is_loaded())
        {
            /* If the suite isn't already loaded, just load it.
                * If load fails, give up and return false.
                */
            if(!load_suite(suite))
            {
                return false;
            }
        }

        // Display the suite name in a banner.
        out << "===== [" << suites[suite]->get_title() << "] =====" << IOCtrl::endl;

        // Run the suite, and return whether it succeeded.
        bool r = suites[suite]->run_tests();

        //If the suite run fails (return false)...
        if(!r)
        {
            out << IOCtrl::n << IOCtrl::clear << IOCat::error << IOFormatTextAttr::bold << IOFormatTextBG::red << IOFormatTextFG::white
                << "SUITE FAILED" << IOCtrl::endl;

            //End of suite, fail.
            return false;
        }
        //If the suite run succeeds (return true)...
        else
        {
            // Let the user know that all went well.
            out << IOCtrl::n << IOCtrl::clear << IOCat::normal << IOFormatTextAttr::bold << IOFormatTextBG::green << IOFormatTextFG::white
                << "SUITE COMPLETE" << IOCtrl::endl;

            //End of suite, success.
            return true;
        }
    }

    /**Measure the approximate number of CPU cycles that a particular
     * test takes to run.
     *
     * WARNING: This tool is not a replacement for a proper code
     * profiler. See the documentation for more information.
     * \param the name of the test to benchmark
     * \param the number of times to run the test
     * (10 - 10,000, default 100). The more repetitions we have,
     * the more accurate the benchmark will be - but bear execution
     * time in mind, as some tests can take a while.
     * \param whether to print all of the output, or just the summaried
     * verdict.
     * \param whether a dead heat is a success result
     * \return true if the first test wins, else false.
     */
    bool run_benchmark(testname_t test, unsigned int repeat = 100,
                        bool showStats = true, bool deadHeat = true)
    {
        // Ensure both tests exist before continuing.
        /* NOTE: If you're getting a segfault in this function, check here!
         * Attempting to access the functions in an unregistered test causes
         * Undefined Behavior, typically a segfault. */
        if(!validate(test, true, GolidlocksItemType::test) || !validate(test, true, GolidlocksItemType::comparative))
        {
            return -1;
        }

        return run_compare(tests[test].get(), comparatives[test].get(), repeat,
            showStats, deadHeat);
    }

    /**Benchmark and compare two tests using a three-pass system.
     * The three passes - MAMA BEAR, PAPA BEAR, and BABY BEAR -
     * handle test execution sequence differently to account for
     * cache warming factors.
     *
     * The tests should be designed to perform the exact same logical
     * task by two different methods.
     *
     * WARNING: This tool is not a replacement for a proper code
     * profiler. See the documentation for more information.
     * \param the name of test A
     * \param the name of test B
     * \param the number of times to run each test per pass
     * (10 - 10,000, default 100). The more repetitions we have,
     * the more accurate the benchmark will be - but bear execution
     * time in mind, as some tests can take a while.
     * In short, we have 100 repetitions each * 2 tests * 3 passes,
     * or 100 => 600 total repetitions.
     * \param whether to print all of the output, or just the summaried
     * verdict.
     * \param whether a dead heat is a success result
     * \return true if the first test wins, else false.
     */
    bool run_compare(testname_t test1, testname_t test2,
                        unsigned int repeat = 100,
                        bool showStats = true, bool deadHeat = true)
    {
        // Ensure both tests exist before continuing.
        /* NOTE: If you're getting a segfault in this function, check here!
            * Attempting to access the functions in an unregistered test causes
            * Undefined Behavior, typically a segfault. */
        if(!validate(test1, true, GolidlocksItemType::test) || !validate(test2, true, GolidlocksItemType::test))
        {
            return false;
        }

        return run_compare(tests[test1].get(), tests[test2].get(), repeat,
            showStats, deadHeat);
    }

    // INTERACTIVE SYSTEM

    /**Interactively (confirm before start) load all suites.
     * \param the suite name to load (or leave empty to load all) */
    void i_load_suite(suitename_t = suite)
    {
        // If no suite was specified (load all)...
        if(suite == "")
        {
            out << "Load ALL test suites? (y/N) " << IOCtrl::end;

            if(i_confirm(false))
            {
                load_suite();
            }
            return;
        }

        // Otherwise, if a suite was specified...

        // Ensure the test exists before continuing.
        if(!validate(suite, true, GolidlocksItemType::suite))
        {
            return;
        }

        if(!suites[suite]->is_loaded())
        {
            out << IOCat::error << IOFormatTextAttr::bold << IOFormatTextFG::red << "ERROR: The suite " << suite
                << " is not yet loaded. Aborting."
                << IOCtrl::endl;
            return;
        }

        out << "Load test suite " << IOCtrl::end;
        out << IOFormatTextAttr::bold << suites[suite]->get_title() << IOCtrl::end;
        out << "[" << suite << "]? (y/N) " << IOCtrl::end;

        if(i_confirm(false))
        {
            load_suite(suite);
        }
    }

    void TestManager::i_run(testname_t item, unsigned int repeat = 1)
    {
        // Don't make this yell on fail...let the next call do it!
        if(validate(item, false, GolidlocksItemType::test))
        {
            i_run_test(item, repeat);
        }
        else if(validate(item, true, GolidlocksItemType::suite))
        {
            i_run_suite(item);
        }
    }

    /**Interactively (confirm before start) run a test by name.
     * \param the name of the test to run*/
    void i_run_test(testname_t test, unsigned int repeat = 1)
    {
        // Ensure the test exists before continuing.
        if(!validate(test, true, GolidlocksItemType::test))
        {
            return;
        }

        out << "Run test " << IOCtrl::end;
        out << IOFormatTextAttr::bold << tests[test]->get_title() << IOCtrl::end;
        out << "[" << test << "]" << IOCtrl::end;
        if(repeat > 1)
        {
            out << " " << stringy::itos(repeat) << " times" << IOCtrl::end;
        }
        out << "? (y/N) " << IOCtrl::end;

        if(i_confirm(false))
        {
            run_test(test, repeat);
        }
    }

    /**Interactively (confirm before start) run a suite by name.
     * \param the name of the suite to run*/
    void i_run_suite(suitename_t suite)
    {
        // Ensure the test exists before continuing.
        if(!validate(suite, true, GolidlocksItemType::suite))
        {
            return;
        }

        if(!suites[suite]->is_loaded())
        {
            /* If the suite isn't already loaded, just load it.
                * If load fails, give up and return.
                */
            if(!load_suite(suite))
            {
                return;
            }
        }

        out << "Run test suite " << IOCtrl::end;
        out << IOFormatTextAttr::bold << suites[suite]->get_title() << IOCtrl::end;
        out << "[" << suite << "]? (y/N) " << IOCtrl::end;

        if(i_confirm(false))
        {
            run_suite(suite);
        }
    }

    /**Interactively run a benchmark by name.
     *
     * \param the name of the test to benchmark
     * \param the number of times to run the test */
    void i_run_benchmark(testname_t test, unsigned int repeat = 100)
    {
        // Ensure the test exists before continuing.
        /* NOTE: If you're getting a segfault in this function, check here!
         * Attempting to access the functions in an unregistered test causes
         * Undefined Behavior, typically a segfault. */
        if(!validate(test, true, GolidlocksItemType::test) || !validate(test, true, GolidlocksItemType::comparative))
        {
            return;
        }

        out << "Run comparative benchmark between " << IOCtrl::end;
        out << IOFormatTextAttr::bold << tests[test]->get_title() << IOCtrl::end;
        out << "[" << test << "] and " << IOCtrl::end;
        out << IOFormatTextAttr::bold << comparatives[test]->get_title() << IOCtrl::end;
        out << " at " << repeat << " repetitions? (y/N) " << IOCtrl::end;

        if(i_confirm(false))
        {
            run_benchmark(test, repeat);
        }
    }

    /**Interactively (confirm before start) run a comparative
     * benchmark by test name.
     * \param the name of test A
     * \param the name of test B
     * \param the number of times to run each test per pass
     * \param whether to print all of the output, or just the summaried
     * verdict */
    void i_run_compare(testname_t test1, testname_t test2,
                        unsigned int repeat = 100, bool showResults = true)
    {
        // Ensure the test exists before continuing.
        /* NOTE: If you're getting a segfault in this function, check here!
         * Attempting to access the functions in an unregistered test causes
         * Undefined Behavior, typically a segfault. */
        if(!validate(test1, true, GolidlocksItemType::test) || !validate(test2, true, GolidlocksItemType::test))
        {
            return;
        }

        out << "Run comparative benchmark between " << IOCtrl::end;
        out << IOFormatTextAttr::bold << tests[test1]->get_title() << IOCtrl::end;
        out << "[" << test1 << "] and " << IOCtrl::end;
        out << IOFormatTextAttr::bold << tests[test2]->get_title() << IOCtrl::end;
        out << "[" << test2 << "] at " << repeat << " repetitions? (y/N) " << IOCtrl::end;

        if(i_confirm(false))
        {
            run_compare(test1, test2, repeat, showResults);
        }
    }

    /**We don't need anything in the destructor, as the smart pointers
     * handle deletion automatically.*/
    ~TestManager() {}

protected:
    /** Confirm y/N from the user. Used by the interactive functions.
     * \param the default option (true=yes, false=no)
     * \return true if confirmed, else false */
    bool i_confirm(bool default_option = false)
    {
        // Get what the user typed.
        std::string buffer;
        getline(std::cin, buffer);

        // If yes was selected...
        if(buffer == "y" || buffer == "Y")
        {
            return true;
        }
        //Otherwise, if no was selected...
        else if(buffer == "n" || buffer == "N")
        {
            return false;
        }
        // For anything else, use the default...
        return default_option;
    }
    }

    /**Validate that the test (by name) is registered with Golidlocks.
     * this is critical in preventing segfaults from accessing
     * invalid tests.
     *
     * \param the name of the test to check for
     * \param whether to display an error message if there is no match
     * \param whether we're searching for a suite (default, test)
     * \return true if the test exists, else false
     */
    bool validate(testname_t item_name, bool yell = false,
                  GolidlocksItemType type = GolidlocksItemType::test)
    {
        /* Use `std::map::count` to test for the key in the map.
            * Using `std::map::find` in this way has some memory issues,
            * according to Clang.
            */
        bool r = false;
        switch(type)
        {
            case GolidlocksItemType::test:
                r = (tests.count(item_name) != 0);
                break;
            case GolidlocksItemType::comparative:
                r = (comparatives.count(item_name) != 0);
                break;
            case GolidlocksItemType::suite:
                r = (suites.count(item_name) != 0);
                break;
        }

        if(yell && !r)
        {
            switch(type)
            {
                case GolidlocksItemType::test:
                case GolidlocksItemType::suite:
                    out << IOCat::error << IOFormatTextAttr::bold << IOFormatTextFG::red << "ERROR: The item " << item_name
                    << " is not registered with Goldilocks Test Manager. Aborting."
                    << IOCtrl::endl;
                    break;
                case GolidlocksItemType::comparative:
                    out << IOCat::error << IOFormatTextAttr::bold << IOFormatTextFG::red << "ERROR: The item " << item_name
                    << " does not have a comparative test registered with Golidlocks Test Manager. Aborting."
                    << IOCtrl::endl;
                    break;
            }
        }


        return r;
    }


    /**Benchmark and compare two tests using a three-pass system.
     * The three passes - MAMA BEAR, PAPA BEAR, and BABY BEAR -
     * handle test execution sequence differently to account for
     * cache warming factors.
     *
     * The tests should be designed to perform the exact same logical
     * task by two different methods.
     *
     * WARNING: This tool is not a replacement for a proper code
     * profiler. See the documentation for more information.
     * \param the name of test A
     * \param the name of test B
     * \param the number of times to run each test per pass
     * (10 - 10,000, default 100). The more repetitions we have,
     * the more accurate the benchmark will be - but bear execution
     * time in mind, as some tests can take a while.
     * In short, we have 100 repetitions each * 2 tests * 3 passes,
     * or 100 => 600 total repetitions.
     * \param whether to print all of the output, or just the summaried
     * verdict.
     * \param whether a dead heat is a success result
     * \return true if the first test wins, else false
     */
    bool run_compare(Test* test1, Test* test2, unsigned int repeat = 100,
                        bool showStats = true, bool deadHeat = true)
    {
        /* ENTRY: Compare two function benchmarks.
            * Largely based on run_benchmark, with several
            * additions for the different comparison types.
            */

        /* We should only allow repeating between 10 and 10,000 times,
            * inclusively. If the user asked for more than that...*/
        if(repeat < 10 || repeat > 10000)
        {
            // Display an error.
            out << IOCat::error << "Benchmark requires a repeat value "
                << "between 10-10000 (inclusively)." << IOCtrl::endl;

            // Abort the benchmark.
            return false;
        }

        // Display the fancy benchmarker banner and disclaimer.
        benchmark_banner();
        // Display the names of the tests we're about to compare.
        out << "===== [" << test1->get_title() << "] | ["
            << test2->get_title() << "] =====" << IOCtrl::endl;

        // Attempt to run the "pre" function for test 1. If it fails...
        if(!(test1->pre()))
        {
            // Alert the user with an error message...
            out << IOCat::error << IOFormatTextAttr::bold << IOFormatTextBG::red << IOFormatTextFG::white
                << "[" << test1->get_title() << "] PRE-TEST FAILED - ABORTING" << IOCtrl::endl;
            // Perform pre-fail cleanup on test 1.
            test1->prefail();

            // Don't worry about test 2 - it hasn't been set up yet.

            // Abort the benchmarker.
            return false;
        }

        // Attempt to run the "pre" function for test 2. If it fails...
        if(!(test2->pre()))
        {
            // Alert the user with an error message...
            out << IOCat::error << IOFormatTextAttr::bold << IOFormatTextBG::red << IOFormatTextFG::white
                << "[" << test2->get_title() << "] PRE-TEST FAILED - ABORTING" << IOCtrl::endl;

            // Perform pre-fail cleanup on test 2.
            test2->prefail();

            /* Perform standard cleanup (post) on test 1. Otherwise, we can
                * potentially wind up with memory leaks and other issues.*/
            test1->post();

            //Abort the benchmarker.
            return false;
        }

        // Let the user know what we're doing...
        out << "Ensuring [" << test1->get_title() << "] succeeds before benchmarking..." << IOCtrl::endl;

        test1->janitor();

        // Run test1 to make sure it works. If it fails...
        if(!(test1->run()) || !(test1->verify()))
        {
            // Alert the user with an error message.
            out << IOCat::error << IOFormatTextAttr::bold << IOFormatTextBG::red << IOFormatTextFG::white
                << "[" << test1->get_title() << "] FAILED - ABORTING" << IOCtrl::endl;

            // Run postmortem cleanup on test1.
            test1->postmortem();

            /* Perform standard cleanup (post) on test 2. Otherwise, we can
                * potentially wind up with memory leaks and other issues.*/
            test2->post();

            //Abort the benchmarker.
            return false;
        }
        // Otherwise, if test 1 succeeds...
        else
        {
            // Let the user know the good news...
            out << IOCat::normal << IOFormatTextAttr::bold << IOFormatTextBG::green << IOFormatTextFG::white
                << "[" << test1->get_title() << "] PASSED" << IOCtrl::endl;
            // ...and move on.
        }

        // Give the user a status update.
        out << "Ensuring [" << test2->get_title() << "] succeeds before benchmarking..." << IOCtrl::endl;

        test2->janitor();

        // Run test2 to make sure it works. If it fails...
        if(!(test2->run()) || !(test2->verify()))
        {
            // Alert the user with an error message.
            out << IOCat::error << IOFormatTextAttr::bold << IOFormatTextBG::red << IOFormatTextFG::white
                << "[" << test2->get_title() << "] FAILED - ABORTING" << IOCtrl::endl;

            // Run postmortem cleanup on test2.
            test2->postmortem();

            /* Perform standard cleanup (post) on test 1. Otherwise, we can
                * potentially wind up with memory leaks and other issues.*/
            test1->post();

            // Abort the benchmarker.
            return false;
        }
        // Otherwise, if test 2 succeeds...
        else
        {
            // Let the user know the good news about that as well...
            out << IOCat::normal << IOFormatTextAttr::bold << IOFormatTextBG::green << IOFormatTextFG::white
                << "[" << test2->get_title() << "] PASSED" << IOCtrl::endl;
            // ...and let the fun proceed!
        }

        // Let the user know that we're starting our measurements.
        out << IOCat::normal << IOFormatTextBG::cyan << IOFormatTextFG::white << "STARTING BENCHMARK" << IOCtrl::endl;

        /* Shut off all testing messages; they aren't useful after the initial
            * test run has finished. We don't want to clog up or clutter output.*/
        out.shutup(IOCat::testing);

        /* The average execution time of the measurement functions, represented
            * as a 64-bit unsigned integer.*/
        uint64_t base;

        // Dynamically allocate two arrays for storing measurements.
        uint64_t* results1 = new uint64_t[repeat];
        uint64_t* results2 = new uint64_t[repeat];

        // If we were unable to allocate either (or both) array...
        if(results1 == nullptr || results2 == nullptr)
        {
            // Alert the user with an error message.
            out << IOCat::error << IOFormatTextAttr::bold << IOFormatTextBG::red << IOFormatTextFG::white
                << "Cannot allocate results arrays. Aborting." << IOCtrl::endl;

            if(results1 != nullptr)
            {
                delete[] results1;
            }

            if(results2 != nullptr)
            {
                delete[] results2;
            }

            // Abort the benchmarker.
            return false;
        }

        // Calibrate our measurement functions. (See that function's comments.)
        calibrate();

        /* Take <repeat> baseline measurements (measuring the measurement
            * function itself.)*/
        for(unsigned int i=0; i<repeat; ++i)
        {
            /* Keep the user apprised of our progress. We use `IOCtrl::endc` so we
                * keep overwriting the same line.*/
            out << IOCat::normal << "CALIBRATING: " << (((i+1)/repeat)*100) << "%" << IOCtrl::endc;
            // Get a baseline measurement.
            results1[i] = clock();
        }

        // Move to a new line for output.
        out << IOCtrl::endl;

        /* Calculate the statistical results from our array of baseline
            * measurements.*/
        BenchmarkResult baseR(results1, repeat);
        /* Store the average baseline measurement for offsetting all our later
            * measurements, thereby measuring JUST the test.*/
        base = baseR.mean;

        /* MAMA BEAR (cold cache) alternates the tests A-B-A-B-A-B, to maximize
            * the possibility of cache misses on each individual test run.*/

        // Display MAMA BEAR banner.
        out << IOCat::normal  << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "COMPARISON 1/3: MAMA BEAR" << IOCtrl::endl;
        out << IOCat::normal << "(MAMA BEAR) Loading..." << IOCtrl::endc;

        // Take <repeat> measurements of test A and B.
        for(unsigned int i=0; i<repeat; ++i)
        {
            // Display progress (overwrite line.)
            out << IOCat::normal << "(MAMA BEAR) Pass " << (i+1) << "-A of " << repeat << ".  " << IOCtrl::endc;
            // Run pre-repeat cleanup on test1.
            test1->janitor();
            // Record test 1 measurement, offset by baseline.
            results1[i] = clock(test1) - base;
            // Run verification on test1.
            test1->verify();

            // Display progress (overwrite line.)
            out << IOCat::normal << "(MAMA BEAR) Pass " << (i+1) << "-B of " << repeat << ".  " << IOCtrl::endc;
            // Run pre-repeat cleanup on test2.
            test2->janitor();
            // Record test 2 measurement, offset by baseline.
            results2[i] = clock(test2) - base;
            // Run verification on test2.
            test2->verify();
        }

        // Move to a new line for output.
        out << IOCtrl::endl;

        // Calculate the statistical results for MAMA BEAR, tests 1 and 2.
        BenchmarkResult mama1(results1, repeat);
        BenchmarkResult mama2 (results2, repeat);

        /* PAPA BEAR (hot cache) performs all runs of test A before test B, to
        * minimize the possibility of cache misses on each individual test run;
        * in other words, allowing natural cache warming to occur.*/

        // Display PAPA BEAR banner.
        out << IOCat::normal  << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "COMPARISON 2/3: PAPA BEAR" << IOCtrl::endl;
        out << IOCat::normal << "(PAPA BEAR) Loading..." << IOCtrl::endc;

        // Take <repeat> measurements of test A.
        for(unsigned int i=0; i<repeat; ++i)
        {
            // Display progress (overwrite line).
            out << IOCat::normal << "(PAPA BEAR) Pass " << (i+1) << "-A of " << repeat << ".  " << IOCtrl::endc;
            // Run pre-repeat cleanup on test1.
            test1->janitor();
            // Record test 1 measurement, offset by baseline.
            results1[i] = clock(test1) - base;
            // Run verification on test1.
            test1->verify();
        }

        // Take <repeat> measurements of test B.
        for(unsigned int i=0; i<repeat; ++i)
        {
            // Display progress (overwrite line).
            out << IOCat::normal << "(PAPA BEAR) Pass " << (i+1) << "-B of " << repeat << ".  " << IOCtrl::endc;
            // Run pre-repeat cleanup on test2.
            test2->janitor();
            // Record test 2 measurement, offset by baseline.
            results2[i] = clock(test2) - base;
            // Run verification on test2.
            test2->verify();
        }

        // Move to a new line for output.
        out << IOCtrl::endl;

        // Calculate the statistical results for PAPA BEAR, tests 1 and 2.
        BenchmarkResult papa1(results1, repeat);
        BenchmarkResult papa2(results2, repeat);

        /* BABY BEAR (warm cache, or "just right"), alternates tests A and B
            * in sets of eight, therefore allowing some cache warming to occur,
            * but also allowing cache misses to be a factor. In some cases, this
            * is closer to what would occur with typical use of an algorithm or
            * approach.*/

        // Display BABY BEAR banner.
        out << IOCat::normal << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "COMPARISON 3/3: BABY BEAR" << IOCtrl::endl;
        out << IOCat::normal << "(BABY BEAR) Loading..." << IOCtrl::endc;

        /* Define three 16-bit unsigned integers for managing BABY BEAR's more
            * complex alternation - counters for the outer and inner loops, and
            * one for all remaining tests when our repeat is not divisible by
            * eight (which it usually won't be.) The inner loop should always be
            * eight!*/
        uint16_t inner=8, outer, remain;
        // The outer loop is the repeat count, integer-divided-by eight.
        outer = repeat / 8;
        // We store the remainder, so we still meet the exact repeat count.
        remain = repeat % 8;

        // We run 8 of each test in each set (outer)...
        for(int o=0; o<outer; o++)
        {
            // Run test 1 eight consecutive times for this set...
            for(int i1=0; i1<inner; i1++)
            {
                // Display progress (overwrite line.)
                out << IOCat::normal << "(BABY BEAR) Pass " << ((o+1)*(i1+1)) << "-A of " << repeat << ".  " << IOCtrl::endc;
                // Run pre-repeat cleanup on test1.
                test1->janitor();
                // Record test 1 measurement, offset by baseline.
                results1[i1] = clock(test1) - base;
                // Run verification on test1.
                test1->verify();
            }

            // Run test 2 eight consecutive times for this set...
            for(int i2=0; i2<inner; i2++)
            {
                // Display progress (overwrite line.)
                out << IOCat::normal << "(BABY BEAR) Pass " << ((o+1)*(i2+1)) << "-B of " << repeat << ".  " << IOCtrl::endc;
                // Run pre-repeat cleanup on test2.
                test2->janitor();
                // Record test 2 measurement, offset by baseline.
                results2[i2] = clock(test2) - base;
                // Run verification on test2.
                test2->verify();
            }
        }

        // Run test 1 the remaining number of times consecutively...
        for(int r1=0; r1<remain; r1++)
        {
            // Display progress (overwrite line.)
            out << IOCat::normal << "(BABY BEAR) Pass " << ((outer*inner)+(r1+1)) << "-A of " << repeat << ".  " << IOCtrl::endc;
            // Run pre-repeat cleanup on test1.
            test1->janitor();
            // Record test 1 measurement, offset by baseline.
            results1[r1] = clock(test1) - base;
            // Run verification on test1.
            test1->verify();
        }

        // Run test 2 the remaining number of times consecutively...
        for(int r2=0; r2<remain; r2++)
        {
            // Display progress (overwrite line.)
            out << IOCat::normal << "(BABY BEAR) Pass " << ((outer*inner)+(r2+1)) << "-B of " << repeat << "." << IOCtrl::sendc;
            // Run pre-repeat cleanup on test2.
            test2->janitor();
            // Record test 2 measurement, offset by baseline.
            results2[r2] = clock(test2) - base;
            // Run verification on test2.
            test2->verify();
        }

        // Move to a new line for output.
        out << IOCtrl::endl;

        // Calculate the statistical results for BABY BEAR, tests 1 and 2.
        BenchmarkResult baby1(results1, repeat);
        BenchmarkResult baby2(results2, repeat);

        // Turn on testing messages again, as we're done running the tests.
        out.speakup(IOCat::testing);

        // Let the user know that we're done running the benchmarker.
        out << IOCat::normal << "\n" << IOFormatTextAttr::bold << IOFormatTextBG::green << IOFormatTextFG::white
                << "BENCHMARKER COMPLETE" << IOCtrl::endl;


        // Cleanup test 1 and 2 using their post functions.
        test1->post();
        test2->post();

        // Display information about results.
        if(showStats)
        {
            out << "RESULTS" << IOCtrl::endl;
            out << "\tMany numbers are displayed as VALUE/ADJUSTED" << IOCtrl::n
                << "\t(The adjusted value excludes outlier values.)" << IOCtrl::n << IOCtrl::endl;

            // Display the baseline measurements, for reference.
            out << IOCat::normal << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "BASELINE MEASUREMENTS" << IOCtrl::endl;
            printResult(baseR);

            // Display the results for MAMA BEAR, test 1.
            out << IOCat::normal << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "MAMA BEAR: [" << test1->get_title() << "]" << IOCtrl::endl;
            printResult(mama1);

            // Display the results for MAMA BEAR, test 2.
            out << IOCat::normal << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "MAMA BEAR: [" << test2->get_title() << "]" << IOCtrl::endl;
            printResult(mama2);

            // Display the verdict for MAMA BEAR in BOLD.
            out << IOCat::normal << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "MAMA BEAR: VERDICT" << IOCtrl::endl;
            out << IOFormatTextAttr::bold;
            printVerdict(mama1, mama2, test1, test2);

            // Display the results for PAPA BEAR, test 1.
            out << IOCat::normal << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "PAPA BEAR: TEST [" << test1->get_title() << "]" << IOCtrl::endl;
            printResult(papa1);

            // Display the results for PAPA BEAR, test 2.
            out << IOCat::normal << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "PAPA BEAR: TEST [" << test2->get_title() << "]" << IOCtrl::endl;
            printResult(papa2);

            // Display the verdict for PAPA BEAR in BOLD.
            out << IOCat::normal << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "PAPA BEAR: VERDICT" << IOCtrl::endl;
            out << IOFormatTextAttr::bold;
            printVerdict(papa1, papa2, test1, test2);

            // Display the results for BABY BEAR, test 1.
            out << IOCat::normal << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "BABY BEAR: TEST [" << test1->get_title() << "]" << IOCtrl::endl;
            printResult(baby1);

            // Display the results for BABY BEAR, test 2.
            out << IOCat::normal << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "BABY BEAR: TEST [" << test2->get_title() << "]" << IOCtrl::endl;
            printResult(baby2);

            // Display the verdict for BABY BEAR in BOLD.
            out << IOCat::normal << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "BABY BEAR: VERDICT" << IOCtrl::endl;
            out << IOFormatTextAttr::bold;
            printVerdict(baby1, baby2, test1, test2);
        }
        else
        {
            // Display the baseline measurements, for reference.
            out << IOCat::normal << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "BASELINE MEASUREMENTS" << IOCtrl::endl;
            printResult(baseR);

            // Display the verdict for MAMA BEAR in BOLD.
            out << IOCat::normal << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "MAMA BEAR: VERDICT" << IOCtrl::endl;
            out << IOFormatTextAttr::bold;
            printVerdict(mama1, mama2, test1, test2);

            // Display the verdict for PAPA BEAR in BOLD.
            out << IOCat::normal << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "PAPA BEAR: VERDICT" << IOCtrl::endl;
            out << IOFormatTextAttr::bold;
            printVerdict(papa1, papa2, test1, test2);

            // Display the verdict for BABY BEAR in BOLD.
            out << IOCat::normal << "\n" << IOFormatTextBG::cyan << IOFormatTextFG::white << "BABY BEAR: VERDICT" << IOCtrl::endl;
            out << IOFormatTextAttr::bold;
            printVerdict(baby1, baby2, test1, test2);
        }

        // Cleanup: Delete the results arrays.
        delete [] results1;
        delete [] results2;
        // Null out the pointers to the results arrays (by habit).
        results1 = NULL;
        results2 = NULL;

        // Calculate which test won (-1=error, 0=dead heat, 1=test A, 2=test B)
        int8_t verdict = calculateVerdict(baby1, baby2);

        // If test A won...
        if (verdict == 1)
        {
            // Report success
            return true;
        }
        // If it was a dead heat, and that counts as a success (from args)...
        else if (deadHeat && verdict == 0)
        {
            return true;
        }

        // All other verdict conditions return false.
        return false;
    }

    /**Print the data from a BenchmarkResult. This uses the
     * standard Channel. Output formatting is retained until the end
     * of the function, so you can modify formatting by preceding this
     * function call with `out << [SOME FORMATTING TAGS] << IOCtrl::send;`
     * \param the BenchmarkResult to output from */
    void printResult(BenchmarkResult& result)
    {
        /* If the adjusted relative standard deviation is greater than this
            * value, the accuracy of the other numbers is in serious doubt, and
            * should be taken with a grain of salt. In that situation, we will
            * highlight the RSD output line in red.*/
        int rsd_threshold = 25;

        //Output the relevant data, keeping formatting until the end.
        out << "\tMEAN (μ): " << result.mean << " / " << result.mean_adj << IOCtrl::sendl;

        out <<  "\tMIN-MAX(RANGE): " << result.min_val << "-" << result.max_val << "(" << result.range << ") / "
            <<  result.min_adj_val << "-" << result.max_adj_val << "(" << result.range_adj <<")" << IOCtrl::sendl;

        out << "\tOUTLIERS: " << result.low_out_major+result.low_out_minor << " LOW, "
            << result.upp_out_major+result.upp_out_minor << " HIGH" << IOCtrl::sendl;

        out << "\tSD (σ): " << IOFormatSignificands(2) << result.std_dev << " / " << result.std_dev_adj << "" << IOCtrl::sendl;

        out << "\t" << (result.rsd_adj > rsd_threshold ? IOFormatTextBG::red : IOFormatTextBG::none)
            << "RSD: " << result.rsd << "% / " << result.rsd_adj << "%" << IOCtrl::endl;
    }

    /** Calculate the final verdict based on adjusted results.
     * \param the BenchmarkResult from test A
     * \param the BenchmarkResult from test B
     * \return 0 if even, 1 if A wins, 2 if B wins.
     */
    uint8_t calculateVerdict(BenchmarkResult& result1, BenchmarkResult& result2)
    {
        // Calculate difference between the adjusted mean averages.
        int64_t difference_adj = result1.mean_adj-result2.mean_adj;

        // If the absolute difference is less than either standard deviation...
        if(labs(difference_adj) <= result1.std_dev_adj || labs(difference_adj) <= result2.std_dev_adj)
        {
            // The tests are roughly the same. Return 0.
            return 0;
        }
        else
        {
            //If the first test won (its adjusted mean was smaller)...
            if(difference_adj < 0)
            {
                // Declare the first test as faster.
                return 1;
            }
            //Else if the second test won (its adjusted mean was smaller)...
            else if(difference_adj > 0)
            {
                // Declare the second test as faster.
                return 2;
            }
        }

        // If we get here, math is broken. Panic and return -1 (error).
        return -1;
    }

    /**Compare and print two BenchmarkResults. This calculates which
     * result was faster, based on the adjusted statistics (outliers
     * removed), and factoring uncertainty into the verdict.
     * Output formatting is retained until the end
     * of the function, so you can modify formatting by preceding this
     * function call with `out << [SOME FORMATTING TAGS] << IOCtrl::send;`
     * \param the BenchmarkResult from test A
     * \param the BenchmarkResult from test B
     * \param the name of test A (optional)
     * \param the name of test B (optional)*/
    void printVerdict(BenchmarkResult& result1, BenchmarkResult& result2,
                      Test* test1, Test* test2)
    {
        // Calculate difference between the non-adjusted mean averages.
        int64_t difference = result1.mean-result2.mean;
        // Calculate difference between the adjusted mean averages.
        int64_t difference_adj = result1.mean_adj-result2.mean_adj;

        // FOR NON-ADJUSTED (RAW) DATA...

        // If the absolute difference is less than either standard deviation...
        if(labs(difference) <= result1.std_dev || labs(difference) <= result2.std_dev)
        {
            //Non-adjusted, the tests are roughly identical.
            out << "\t     RAW: Both tests are roughly identical. (DIFF <= STD DEV)" << IOCtrl::sendl;
        }
        else
        {
            // If the first test won (its non-adjusted mean was smaller)...
            if(difference < 0)
            {
                // Declare the first test as faster, and by how much.
                out << "\t     RAW: [" << test1->get_title() << "] faster by approx. " << (labs(difference)) << " cycles." << IOCtrl::sendl;
            }
            //Else if the second test won (its non-adjusted mean was smaller)...
            else if(difference > 0)
            {
                // Declare the second test as faster, and by how much.
                out << "\t     RAW: [" << test2->get_title() << "] faster by approx. " << (labs(difference)) << " cycles." << IOCtrl::sendl;
            }
        }

        // FOR ADJUSTED DATA...

        // If the absolute difference is less than either standard deviation...
        if(labs(difference_adj) <= result1.std_dev_adj || labs(difference_adj) <= result2.std_dev_adj)
        {
            //Adjusted, the tests are roughly identical.
            out << "\tADJUSTED: Both tests are roughly identical. (DIFF <= STD DEV)" << IOCtrl::endl;
        }
        else
        {
            //If the first test won (its adjusted mean was smaller)...
            if(difference_adj < 0)
            {
                // Declare the first test as faster, and by how much.
                out << "\tADJUSTED: [" << test1->get_title() << "] faster by approx. " << (labs(difference_adj) - result1.std_dev_adj) << " cycles." << IOCtrl::endl;
            }
            //Else if the second test won (its adjusted mean was smaller)...
            else if(difference_adj > 0)
            {
                // Declare the second test as faster, and by how much.
                out << "\tADJUSTED: [" << test2->get_title() << "] faster by approx. " << (labs(difference_adj) - result2.std_dev_adj) << " cycles." << IOCtrl::endl;
            }
        }
    }

    /** Print the banner for the benchmarker functions using Channel.*/
    void benchmark_banner()
    {
        // Display the fancy benchmark banner, with its disclaimer.
        out << IOFormatTextAttr::bold << IOFormatTextFG::blue
            << "======" << "===========" << "=====\n"
            << "|     " << "BENCHMARKER" << "    |\n"
            << "======" << "===========" << "=====" << IOCtrl::endl;
        out << IOFormatTextAttr::none << IOFormatTextFG::red
            << "Disclaimer: This benchmarker is intended to give a rough estimate "
            << "of how many CPU cycles a particular test takes. It is not a "
            << "replacement for a fully-featured code profiler. "
            << "Cache warming will NOT be performed.\n"
            << IOCtrl::endl;
    }
};

#endif // GOLDILOCKS_MANAGER_HPP
