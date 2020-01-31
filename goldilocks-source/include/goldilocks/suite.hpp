/** Suite [Goldilocks]
 * Version: 2.0
 *
 * A Suite is responsible for organizing Tests, allowing them to be loaded
 * and run on-demand. Suites also report what tests it contains.
 *
 * Author(s): Jason C. McDonald
 */

/* LICENSE (BSD-3-Clause)
 * Copyright (c) 2016-2020 MousePaw Media.
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

#ifndef GOLDILOCKS_SUITE_HPP
#define GOLDILOCKS_SUITE_HPP

#include "goldilocks/test.hpp"
#include "goldilocks/types.hpp"

class TestManager;

class Suite
{
    friend class TestManager;
protected:
    /** Stores all of the test pointers for access-by-name-string. */
    std::map<testname_t, testptr_t> tests;

    /** Stores all of the comparative test pointers for
     * access-by-name-string. */
    std::map<testname_t, testptr_t> comparatives;

public:
    Suite() : loaded(false) {}

    /** Load all of the tests into the testsystem and suite.
     */
    virtual void load_tests() = 0;

    virtual testdoc_t get_title() = 0;

    /** Run all of the tests in the suite.
     * \return true if all the tests run, else false
     */
    bool run_tests() final
    {
        // If we have a valid testmanager and tests to run...
        if(testmanager && tests_run.size() > 0)
        {
            // Run each test in this suite through the test manager.
            for(unsigned int i = 0; i < tests_run.size(); ++i)
            {
                // If a test fails...
                if(!testmanager->run_test(tests_run[i]))
                {
                    // Don't bother running the rest, just fail.
                    return false;
                }
            }
            // If we reach this point, all of the tests passed; report success.
            return true;
        }
        // Else if we don't have any tests marked to run...
        else if(tests_run.size())
        {
            out << IOCat::error << IOVrb::normal
                << "ERROR: Suite has no tests marked to run. Aborting."
                << IOCtrl::endl;
            return false;
        }
        // Else if we don't have a valid testmananger...
        else if(!testmanager)
        {
            out << IOCat::error << IOVrb::normal
                << "ERROR: Suite is lonely and has no one to talk to. :("
                << IOCtrl::endl;
            return false;
        }

        // If we reach this point, something weird was wrong; report failure.
        return false;
    }

    /** Check if the Suite was already loaded, or mark it as loaded.
     * Used directly by Goldilocks.
     * \param whether to mark the suite as loaded
     * \return true if loaded, else false */
    bool is_loaded(bool mark = false) final
    {
        if (mark)
        {
            loaded = true;
        }
        return loaded;
    }

    virtual ~Suite() {}

protected:
    /** Register a test with the suite.
     * This is here purely for end-developer convenience when creating
     * a derived Suite.
     * \param name of the test
     * \param pointer to the test (declare "new" for this argument)
     * \param whether to run the test on suite run (default true)
     */
    void register_test(testname_t test_name, Test* test, bool will_run = true, Test* compare = 0)
    {
        // If we have a valid testmanager pointer...
        if(testmanager)
        {
            // Register the test with the testmanager.
            /* FIXME: Remove "backregister" pattern and just store tests in
             * Suite. Have Manager share pointers/names managed by Suites.
             */
            testmanager->register_test(test_name, test, compare);
            // Store the test name for loading on demand.
            tests_load.push_back(test_name);

            // If we're supposed to run the test...
            if(will_run)
            {
                tests_run.push_back(test_name);
            }
        }
        else
        {
            out << IOCat::error << IOVrb::normal
                << "ERROR: Suite is lonely and has no one to talk to. :("
                << IOCtrl::endl;
            return;
        }
    }

    /** The list of tests to run. This should always be a subset
     * of tests_load. */
    std::vector<testname_t> tests_run;

    /** Tracks whether we've already loaded this suite, to prevent
     * loading multiple times into one testmanager. */
    bool loaded;

/* The Suite smart pointer type shall henceforth be known
 * as "suiteptr_t".*/
typedef std::unique_ptr<Suite> suiteptr_t;

#endif // GOLDILOCKS_SUITE_HPP
