/** Test [Goldilocks]
 * Version: 2.0
 *
 * The base class for all Goldilocks tests.
 *
 * Author(s): Wilfrantz DEDE, Jason C. McDonald
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

#ifndef GOLDILOCKS_TEST_HPP
#define GOLDILOCKS_TEST_HPP

#include "goldilocks/types.hpp"
#include "goldilocks/expect.hpp"

#define REQUIRE(expect) \
    do { if (!this->report(expect)) { return false; } } while(0)

#define UNLESS(expect) \
    do { if (this->report(expect)) { return false; } } while(0)

#define CHECK(expect) \
    do { this->report(expect) } while(0)

/** All tests are derived from this base
 * class.*/
class Test
{
private:
    testdoc_t test_name;
    testdoc_t doc_string;
    
public:
    /**A Test should perform most setup tasks, such as dynamic allocation,
     * in `pre()`, so `prefail()`, `post()`, and `postmortem()` can handle
     * cleanup depending on the test's success. */
    Test (testdoc_t test_name, testdoc_t doc_string)
    : test_name(test_name), doc_string(doc_string)
    {}

    /**Set up for the test. Called only once, even if test is
     * repeated multiple times.
     * If undefined, always returns true.
     * \return true if successful, false if it fails.*/
    virtual bool pre() { return true; }

    /**Clean up from a failed pre-test.
     * If undefined, calls post. */
    virtual void prefail() {this->post();}

    /** Clean up between successful runs, in preparation for a repeat.
     * Always executed before run() or run_optimized(), even on the first pass.
     * If undefined, always returns true.*/
    virtual bool janitor() { return true; }

    /**Run test.
     * \return true if successful, false if it fails (error).*/
    virtual bool run() = 0;

    /** Benchmark-optimized form of the test. May be needed if the
     * validity testing will throw off the benchmark.
     * If undefined, executes run()
     * \return true if successful, false if it fails (error). */
    virtual bool run_optimized() { return this->run(); }

    /**Clean up after successful test.
     * If undefined, returns nothing.*/
    virtual void post() {}

    /**Clean up after a failed test.
     * If undefined, calls post() */
    virtual void postmortem() {this->post();}

    /**Like the constructor, a destructor is unnecessary for a Test.
     * Cleanup should be handled by `prefail()`, `post()`, and
     * `postmortem()`, depending on the test's success.
     * In short, THIS SHOULD ALWAYS BE EMPTY!*/
    virtual ~Test() = default;
};

/* The Test smart pointer type shall henceforth be known
 * as "testptr_t".*/
typedef std::unique_ptr<Test> testptr_t;

#endif //GOLDILOCKS_TEST_HPP
