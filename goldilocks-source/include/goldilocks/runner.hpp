/** Runner [Goldilocks]
 * Version: 2.0
 *
 * A Runner executes the actual tests.
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

#ifndef GOLDILOCKS_RUNNER_HPP
#define GOLDILOCKS_RUNNER_HPP

#include "iosqueak/channel.hpp"

#include "goldilocks/test.hpp"
#include "goldilocks/types.hpp"

class Runner
{
protected:
    Test* test;
    Test* comparative;

    unsigned int successes;
    TestStatus status;

    /// Display the test name in a banner.
    void display_banner()
    {
        out << "===== [" << tests[test]->get_title() << "] ====="
            << IOCtrl::endl;
    }

    void display_error(char* message)
    {
        out << IOCat::error
            << IOFormatTextAttr::bold
            << IOFormatTextBG::red << IOFormatTextFG::white
            << message
            << IOCtrl::endl;
    }

    void display_success(char* message)
    {
        out << IOCat::normal
            << IOFormatTextAttr::bold
            << IOFormatTextBG::green << IOFormatTextFG::white
            << message
            << IOCtrl::endl;
    }

public:
    /**Create a new test runner.
     * \param test to run
     * \param optional comparative test to run against
     */
    Runner(Test* test, Test* comparative = nullptr)
    : test(test), comparative(comparative),
      successes(0), status(TestStatus::OK)
    {}

    TestStatus run(unsigned int repeat = 1)
    {
        if(status != TestStatus::OK)
        {
            return TestStatus::CONFUSED;
        }

        if(repeat < 1 || repeat > 10000)
        {
            out << IOCat::error
                << "Test repeat value must be between 1 and 10000 inclusively."
                << IOCtrl::endl;

            // Abort the runner
            status = TestStatus::CONFUSED;
            return status;
        }

        display_banner();

        if(!test->pre())
        {
            // Run pre-fail task.
            test->prefail();

            // Abort runner at pre-test.
            display_error("PRE-TEST FAILED. Aborting.");
            status = TestStatus::PREFAIL;
            return status;
        }

        for(unsigned int i = 0; i < repeat; ++i)
        {
            // Print iteration number.
            out << IOCat::normal
                << "Pass " << (i+1)
                << " of " << (repeat)
                << IOCtrl::endl;

            // Run between-test task.
            test->janitor();

            if(!test->run() || !test->verify())
            {
                status = TestStatus::FAIL;
                break;
            }
        }

        // If the test run fails...
        if(status == TestStatus::FAIL)
        {
            display_error("TEST FAILED.");
            test->postmortem();
            return status;
        }

        // If the test run succeeds...
        display_success("TEST COMPLETE");
        // Run post-test task.
        test->post();

        return TestStatus::OK;

    }

};

#endif // GOLDILOCKS_RUNNER_HPP
